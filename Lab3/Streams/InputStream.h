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

		uint8_t byte;
		m_file.read(reinterpret_cast<char*>(&byte), 1);

		if (IsEOF())
		{
			throw std::ios_base::failure("End of file reached\n");
		}

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
		std::memcpy(dstBuffer, &m_data[m_position], static_cast<size_t>(byteCountForRead));
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

class DecryptInputStreamDecorator : public IInputDataStream
{
public:
	using IInputDataStreamPtr = std::unique_ptr<IInputDataStream>;

	DecryptInputStreamDecorator(IInputDataStreamPtr&& inputStream, uint8_t key) :
		m_inputStream(move(inputStream))
	{
		GenerateDecryptTable(key);
	}

	uint8_t ReadByte() override
	{
		return m_decryptTable[m_inputStream->ReadByte()];
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

		std::streamsize read = m_inputStream->ReadBlock(dstBuffer, size);
		uint8_t* ptr = static_cast<uint8_t*>(dstBuffer);
		for (std::streamsize i = 0; i < read; ++i)
		{
			ptr[i] = m_decryptTable[ptr[i]];
		}

		return read;
	}

	bool IsEOF()const override
	{
		return m_inputStream->IsEOF();
	}

	void Close() override
	{
		m_inputStream->Close();
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
	IInputDataStreamPtr m_inputStream;
};

class DecompressInputStreamDecorator : public IInputDataStream
{
public:
	using IInputDataStreamPtr = std::unique_ptr<IInputDataStream>;

	DecompressInputStreamDecorator(IInputDataStreamPtr&& inputStream) //TODO EBLANI
		:m_inputStream(move(inputStream))
	{
	}

	uint8_t ReadByte() override
	{
		if (m_bytesLeft == 0)
		{
			uint8_t count;
			count = m_inputStream->ReadByte();

			m_currentByte = m_inputStream->ReadByte();

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

		uint8_t* ptr = static_cast<uint8_t*>(dstBuffer);
		std::streamsize readed = 0;

		while (readed < size)
		{
			try
			{
				ptr[readed] = ReadByte();
				++readed;
			}
			catch (const std::ios_base::failure& e)
			{
				break;
			}
		}

		return readed;
	}

	bool IsEOF() const override
	{
		if (m_bytesLeft > 0)
		{
			return false;
		}

		return m_inputStream->IsEOF();
	}

	void Close() override
	{
		m_inputStream->Close();
	}

private:
	uint8_t m_currentByte = 0;
	uint8_t m_bytesLeft = 0;
	IInputDataStreamPtr m_inputStream;
};


