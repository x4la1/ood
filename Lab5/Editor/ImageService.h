#pragma once
#include <string>
#include <filesystem>
#include <random>

namespace fs = std::filesystem;

class ImageService
{
public:
	ImageService(const fs::path& directoryPath)
		:m_storageDir(directoryPath)
	{
		if (!fs::exists(m_storageDir))
		{
			fs::create_directories(m_storageDir);
		}
	}

	std::string AddImage(const fs::path& sourcePath)
	{
		if (!fs::exists(sourcePath))
		{
			throw std::invalid_argument("File does not exist");
		}

		std::string extension = sourcePath.extension().string();
		std::string uniqueName = GenerateUniqueName() + extension;
		fs::path targetPath = m_storageDir / uniqueName;

		fs::copy_file(sourcePath, targetPath, fs::copy_options::overwrite_existing);

		return targetPath.string();
	}

	void DeleteImage(const fs::path& path)
	{
		if (fs::exists(path)) 
		{
			fs::remove(path);
		}
	}

	void ClearAll()
	{
		if (fs::exists(m_storageDir))
		{
			fs::remove_all(m_storageDir);
			fs::create_directories(m_storageDir);
		}
	}

	~ImageService()
	{
		ClearAll();
	}

private:
	fs::path m_storageDir;

	std::string GenerateUniqueName()
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<uint64_t> dis;

		std::stringstream ss;
		ss << "img_" << std::hex << dis(gen);
		return ss.str();
	}
};