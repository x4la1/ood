#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include "InputStream.h"
#include "OutputStream.h"


std::vector<uint8_t> GenerateEncryptTable(int key)
{
	std::vector<uint8_t> table(256);
	std::iota(table.begin(), table.end(), 0);
	std::mt19937 gen(static_cast<unsigned>(key));
	std::shuffle(table.begin(), table.end(), gen);
	return table;
}

std::vector<uint8_t> GenerateDecryptTable(int key) {
	auto enc = GenerateEncryptTable(key);
	std::vector<uint8_t> dec(256);
	for (size_t i = 0; i < 256; ++i)
	{
		dec[enc[i]] = static_cast<uint8_t>(i);
	}
	return dec;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: transform [options] <input-file> <output-file>\n";
		return 1;
	}

	std::vector<std::pair<std::string, int>> input_ops;
	std::vector<std::pair<std::string, int>> output_ops;

	int i = 1;
	while (i < argc - 2) {
		std::string opt = argv[i];
		if (opt == "--encrypt") {
			if (++i >= argc - 2) {
				std::cerr << "Missing key for --encrypt\n";
				return 1;
			}
			int key = std::stoi(argv[i]);
			output_ops.emplace_back("encrypt", key);
		}
		else if (opt == "--decrypt") {
			if (++i >= argc - 2) {
				std::cerr << "Missing key for --decrypt\n";
				return 1;
			}
			int key = std::stoi(argv[i]);
			input_ops.emplace_back("decrypt", key);
		}
		else if (opt == "--compress") {
			output_ops.emplace_back("compress", -1);
		}
		else if (opt == "--decompress") {
			input_ops.emplace_back("decompress", -1);
		}
		else {
			std::cerr << "Unknown option: " << opt << "\n";
			return 1;
		}
		++i;
	}

	std::string input_file = argv[argc - 2];
	std::string output_file = argv[argc - 1];

	std::unique_ptr<IInputDataStream> input_stream = std::make_unique<FileInputStream>(input_file);
	for (const auto& op : input_ops) {
		if (op.first == "decrypt") {
			// ИСПРАВЛЕНИЕ: правильный порядок параметров - сначала поток, потом ключ
			input_stream = std::make_unique<DecryptInputStreamDecorator>(std::move(input_stream), op.second);
		}
		else if (op.first == "decompress") {
			input_stream = std::make_unique<DecompressInputStreamDecorator>(std::move(input_stream));
		}
	}

	// Build output stream with decorators in reverse order
	std::unique_ptr<IOutputDataStream> output_stream = std::make_unique<FileOutputStream>(output_file);
	for (auto it = output_ops.rbegin(); it != output_ops.rend(); ++it) {
		const auto& op = *it;
		if (op.first == "encrypt") {
			// ИСПРАВЛЕНИЕ: правильный порядок параметров - сначала поток, потом ключ
			output_stream = std::make_unique<EncryptOutputStreamDecorator>(std::move(output_stream), op.second);
		}
		else if (op.first == "compress") {
			output_stream = std::make_unique<CompressOutputStreamDecorator>(std::move(output_stream));
		}
	}

	// Transfer data
	constexpr std::streamsize BUF_SIZE = 4096;
	std::vector<uint8_t> buffer(BUF_SIZE);
	try {
		while (!input_stream->IsEOF()) {
			std::streamsize read = input_stream->ReadBlock(buffer.data(), BUF_SIZE);
			output_stream->WriteBlock(buffer.data(), read);
		}
		output_stream->Close();
	}
	catch (const std::exception& e) {
		std::cerr << "Error during transformation: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}


