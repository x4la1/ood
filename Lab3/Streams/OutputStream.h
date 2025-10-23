#pragma once
#include "IOutputDataStream.h"
#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <numeric>


class FileOutputStream : public IOutputDataStream
{
public:
	FileOutputStream(const std::string& path)
	{
		m_file.open(path, std::ios::binary);
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("Can't open file\n");
		}
		m_isOpen = true;
	}

	void WriteByte(uint8_t data) override
	{
		CheckIsOpen();
		m_file.write(reinterpret_cast<const char*>(&data), 1);
		CheckIsFail();
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		CheckIsOpen();
		if (srcData == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}
		m_file.write(static_cast<const char*>(srcData), size);
		CheckIsFail();
	}

	void Close() override
	{
		m_file.close();
		m_isOpen = false;
	};


private:
	void CheckIsOpen() const
	{
		if (!m_isOpen)
		{
			throw std::logic_error("File already closed\n");
		}
	}

	void CheckIsFail() const
	{
		if (m_file.fail())
		{
			throw std::ios_base::failure("Write error");
		}
	}

	std::ofstream m_file;
	bool m_isOpen = false;
};


class MemoryOutputStream : public IOutputDataStream
{
public:
	MemoryOutputStream(std::vector<uint8_t>& data)
		:m_data(&data), m_isOpen(true)
	{
	}

	void WriteByte(uint8_t data) override
	{
		CheckIsOpen();
		m_data->push_back(data);
	}


	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		CheckIsOpen();

		if (srcData == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		const uint8_t* p = static_cast<const uint8_t*>(srcData);
		m_data->insert(m_data->end(), p, p + size);
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

	std::vector<uint8_t>* m_data;
	bool m_isOpen = true;
};

class EncryptOutputStreamDecorator :public IOutputDataStream
{
public:
	using IOutputDataStreamPtr = std::unique_ptr<IOutputDataStream>;

	EncryptOutputStreamDecorator(IOutputDataStreamPtr&& outputStream, uint8_t key) :
		m_outputStream(move(outputStream))
	{
		GenerateEncryptTable(key);
	}

	void WriteByte(uint8_t data) override
	{
		m_outputStream->WriteByte(m_encryptTable[data]);
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		if (srcData == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		std::vector<uint8_t> buffer(static_cast<const unsigned int>(size));
		const uint8_t* src = static_cast<const uint8_t*>(srcData);
		for (int i = 0; i < size; ++i)
		{
			buffer[i] = m_encryptTable[src[i]];
		}

		m_outputStream->WriteBlock(buffer.data(), size);
	}

	void Close() override
	{
		m_outputStream->Close();
	}

private:
	void GenerateEncryptTable(uint8_t key)
	{
		std::vector<uint8_t> encryptTable(256);
		std::iota(encryptTable.begin(), encryptTable.end(), 0);
		std::mt19937 gen(static_cast<unsigned>(key));
		std::shuffle(encryptTable.begin(), encryptTable.end(), gen);
		m_encryptTable = encryptTable;
	}

	std::vector<uint8_t> m_encryptTable;
	IOutputDataStreamPtr m_outputStream;
};


class CompressOutputStreamDecorator : public IOutputDataStream
{
public:
	using IOutputDataStreamPtr = std::unique_ptr<IOutputDataStream>;

	CompressOutputStreamDecorator(IOutputDataStreamPtr&& outputStream)
		:m_outputStream(move(outputStream))
	{
	}

	void WriteByte(uint8_t data) override
	{
		if (m_runLength == 0)
		{
			m_currentByte = data;
			m_runLength = 1;
		}
		else if (data == m_currentByte)
		{
			++m_runLength;
			if (m_runLength == 255)
			{
				Update();
			}
		}
		else {
			Update();
			m_currentByte = data;
			m_runLength = 1;
		}
	}

	void WriteBlock(const void* srcData, std::streamsize size) override
	{
		if (srcData == nullptr)
		{
			throw std::ios_base::failure("Invalid buffer\n");
		}

		if (size < 0)
		{
			throw std::ios_base::failure("Invalid size\n");
		}

		const uint8_t* src = static_cast<const uint8_t*>(srcData);
		for (std::streamsize i = 0; i < size; ++i)
		{
			WriteByte(src[i]);
		}
	}

	void Close() override
	{
		Update();
		m_outputStream->Close();
	}

private:
	void Update()
	{
		if (m_runLength > 0)
		{
			m_outputStream->WriteByte(m_runLength);
			m_outputStream->WriteByte(m_currentByte);
			m_runLength = 0;
		}
	}

	IOutputDataStreamPtr m_outputStream;
	uint8_t m_runLength = 0;
	uint8_t m_currentByte = 0;
};