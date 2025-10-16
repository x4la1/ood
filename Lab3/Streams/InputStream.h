#pragma once
#include "IInputDataStream.h"
#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>


class FileInputStream : public IInputDataStream
{
public:
	FileInputStream(const std::string& path)
	{
		m_file.open(path, std::ios::binary);
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("Can't open file\n");
		}
		m_isOpen = true;
	}

	bool IsEOF() const override
	{
		CheckIsOpen();
		return m_file.eof();
	}

	uint8_t ReadByte() override
	{
		CheckIsOpen();

		if (IsEOF())
		{
			throw std::ios_base::failure("End of file reached\n");
		}

		uint8_t byte;
		m_file.read(reinterpret_cast<char*>(&byte), 1);

		if (m_file.fail())
		{
			throw std::ios_base::failure("Read error\n");
		}

		return byte;
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		CheckIsOpen();

		if (dstBuffer == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		if (IsEOF())
		{
			throw std::ios_base::failure("End of file reached\n");
		}

		m_file.read(static_cast<char*>(dstBuffer), size);
		if (m_file.fail() && !IsEOF())
		{
			throw std::ios_base::failure("Read error\n");
		}

		return m_file.gcount();
	}

	void Close() override
	{
		m_file.close();
		m_isOpen = false;
	}

private:

	void CheckIsOpen() const
	{
		if (!m_isOpen)
		{
			throw std::logic_error("File already closed\n");
		}
	}

	std::ifstream m_file;
	bool m_isOpen = false;
};


class MemoryInputStream : public IInputDataStream
{
public:
	MemoryInputStream(std::vector<uint8_t> data)
		: m_data(std::move(data)), m_position(0), m_isOpen(true)
	{}

	bool IsEOF() const override
	{
		if (!m_isOpen)
		{
			throw std::logic_error("File already closed\n");
		}

		return m_position >= m_data.size();
	}

	uint8_t ReadByte() override
	{
		CheckIsOpen();

		if (IsEOF())
		{
			throw std::ios_base::failure("End of stream reached\n");
		}

		return m_data[m_position++];
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		CheckIsOpen();

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		if (dstBuffer == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (IsEOF())
		{
			throw std::ios_base::failure("End of stream reached\n");
		}

		size_t byteCountLeft = m_data.size() - m_position;
		std::streamsize byteCountForRead = std::min(size, static_cast<std::streamsize>(byteCountLeft));
		std::memcpy(dstBuffer, &m_data[m_position], byteCountForRead);
		m_position += byteCountForRead;

		return byteCountForRead;
	}

	void Close() override
	{
		m_isOpen = false;
	}

private:

	void CheckIsOpen() const
	{
		if (!m_isOpen)
		{
			throw std::logic_error("File already closed\n");
		}
	}

	std::vector<uint8_t> m_data;
	size_t m_position;
	bool m_isOpen;
};


class InputStreamDecorator : public IInputDataStream
{
public:
	using IInputDataStreamPtr = std::unique_ptr<IInputDataStream>;

	bool IsEOF() const override final
	{
		return m_inputStream->IsEOF();
	}

	uint8_t ReadByte() override
	{
		return m_inputStream->ReadByte();
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		return m_inputStream->ReadBlock(dstBuffer, size);
	}

	void Close() override final
	{
		m_inputStream->Close();
	}

protected:
	InputStreamDecorator(IInputDataStreamPtr&& inputStream)
		: m_inputStream(move(inputStream))
	{
	}

private:
	IInputDataStreamPtr m_inputStream;
};

class DecryptInputStreamDecorator : public InputStreamDecorator
{
public:
	DecryptInputStreamDecorator(IInputDataStreamPtr&& inputStream, uint8_t key)
		:InputStreamDecorator(move(inputStream))
	{
		GenerateDecryptTable(key);
	}

	uint8_t ReadByte() override
	{
		return m_decryptTable[InputStreamDecorator::ReadByte()];
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		if (dstBuffer == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		std::streamsize read = InputStreamDecorator::ReadBlock(dstBuffer, size);
		uint8_t* ptr = static_cast<uint8_t*>(dstBuffer);
		for (std::streamsize i = 0; i < read; ++i)
		{
			ptr[i] = m_decryptTable[ptr[i]];
		}

		return read;
	}

private:
	void GenerateDecryptTable(uint8_t key)
	{
		std::vector<uint8_t> encryptTable(256);
		std::vector<uint8_t> decryptTable(256);
		std::iota(encryptTable.begin(), encryptTable.end(), 0);
		std::mt19937 gen(static_cast<unsigned>(key));
		std::shuffle(encryptTable.begin(), encryptTable.end(), gen);

		for (size_t i = 0; i < 256; ++i)
		{
			decryptTable[encryptTable[i]] = static_cast<uint8_t>(i);
		}

		m_decryptTable = decryptTable;
	}

	std::vector<uint8_t> m_decryptTable;
};

class DecompressInputStreamDecorator : public InputStreamDecorator
{
public:
	DecompressInputStreamDecorator(IInputDataStreamPtr&& inputStream)
		:InputStreamDecorator(move(inputStream))
	{
	}

	uint8_t ReadByte() override
	{
		if (m_bytesLeft == 0) {

			if (InputStreamDecorator::IsEOF())
			{
				throw std::ios_base::failure("End of stream reached");
			}

			uint8_t count;
			count = InputStreamDecorator::ReadByte();

			if (count == 0)
			{
				throw std::ios_base::failure("End of compressed data");
			}

			if (InputStreamDecorator::IsEOF()) {
				throw std::ios_base::failure("Unexpected EOF while reading byte");
			}

			try {
				m_currentByte = InputStreamDecorator::ReadByte();
			}
			catch (const std::ios_base::failure&) {
				throw std::ios_base::failure("Unexpected EOF while reading byte");
			}

			m_bytesLeft = count - 1;
			return m_currentByte;
		}

		--m_bytesLeft;
		return m_currentByte;
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		if (dstBuffer == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		if (IsEOF())
		{
			return 0;
		}

		uint8_t* ptr = static_cast<uint8_t*>(dstBuffer);
		std::streamsize readed = 0;

		try {
			while (readed < size && !IsEOF())
			{
				ptr[readed] = ReadByte();
				++readed;
			}
		}
		catch (const std::ios_base::failure& e)
		{
			if (readed == 0 && !IsEOF())
			{
				throw;
			}
		}

		return readed;
	}

private:
	uint8_t m_currentByte = 0;
	uint8_t m_bytesLeft = 0;
};


