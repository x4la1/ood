#include "InputStream.h"
#include "OutputStream.h"

class FileTransformer
{
public:
	FileTransformer(const std::vector<std::string>& arguments)
	{
		ParseArguments(arguments);
	}

	void Transform()
	{
		CreateInputStream();
		CreateOutputStream();
		TransferData();
	}

private:

	void ParseArguments(const std::vector<std::string>& arguments)
	{
		int i = 0;
		const int argumentsCount = arguments.size();
		std::string option;

		if (argumentsCount < 2)
		{
			throw std::invalid_argument("Invalid arguments\n");
		}

		while (i < argumentsCount - 2)
		{
			option = arguments[i];
			if (option == "--encrypt")
			{
				if (++i >= argumentsCount - 2)
				{
					throw std::invalid_argument("Missing key for encrypt\n");
				}
				unsigned int key = TryParseKey(arguments[i]);
				m_outputArguments.emplace_back("encrypt", static_cast<int>(key));
			}
			else if (option == "--decrypt")
			{
				if (++i >= argumentsCount - 2)
				{
					throw std::invalid_argument("Missing key for decrypt\n");
				}
				unsigned int key = TryParseKey(arguments[i]);
				m_inputArguments.emplace_back("decrypt", static_cast<int>(key));
			}
			else if (option == "--compress")
			{
				m_outputArguments.emplace_back("compress", -1);
			}
			else if (option == "--decompress")
			{
				m_inputArguments.emplace_back("decompress", -1);
			}
			else
			{
				throw std::invalid_argument("Unkown option\n");
			}
			++i;
		}

		m_inputFilePath = arguments[argumentsCount - 2];
		m_outputFilePath = arguments[argumentsCount - 1];
	}

	unsigned int TryParseKey(std::string str)
	{
		unsigned int value;
		try
		{
			value = static_cast<unsigned int>(std::stoul(str));
		}
		catch (const std::exception& e)
		{
			throw std::invalid_argument("Invalid key\n");
		}

		return value;
	}

	void CreateInputStream()
	{
		std::unique_ptr<IInputDataStream> inputStream = std::make_unique<FileInputStream>(m_inputFilePath);
		for (const auto& op : m_inputArguments)
		{
			if (op.first == "decrypt")
			{
				inputStream = std::make_unique<DecryptInputStreamDecorator>(std::move(inputStream), op.second);
			}
			else if (op.first == "decompress")
			{
				inputStream = std::make_unique<DecompressInputStreamDecorator>(std::move(inputStream));
			}
		}
		m_inputStream = move(inputStream);
	}

	void CreateOutputStream()
	{
		std::unique_ptr<IOutputDataStream> outputStream = std::make_unique<FileOutputStream>(m_outputFilePath);
		for (auto it = m_outputArguments.rbegin(); it != m_outputArguments.rend(); ++it)
		{
			const auto& op = *it;
			if (op.first == "encrypt")
			{
				outputStream = std::make_unique<EncryptOutputStreamDecorator>(std::move(outputStream), op.second);
			}
			else if (op.first == "compress")
			{
				outputStream = std::make_unique<CompressOutputStreamDecorator>(std::move(outputStream));
			}
		}
		m_outputStream = move(outputStream);
	}

	void TransferData()
	{
		const std::streamsize bufSize = 1024;
		std::vector<uint8_t> buffer(bufSize);

		while (!m_inputStream->IsEOF())
		{
			std::streamsize read = m_inputStream->ReadBlock(buffer.data(), bufSize);
			m_outputStream->WriteBlock(buffer.data(), read);
		}
		m_outputStream->Close();
	}

	using IInputDataStreamPtr = std::unique_ptr<IInputDataStream>;
	using IOutputDataStreamPtr = std::unique_ptr<IOutputDataStream>;

	IInputDataStreamPtr m_inputStream;
	IOutputDataStreamPtr m_outputStream;

	std::string m_inputFilePath;
	std::string m_outputFilePath;
	std::vector<std::pair<std::string, int>> m_inputArguments;
	std::vector<std::pair<std::string, int>> m_outputArguments;
};