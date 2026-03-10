#pragma once
#include <filesystem>

class IImage
{
public:
	virtual std::string GetPath() const = 0;
	virtual std::string GetFileName() const = 0;

	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual void Resize(uint32_t width, uint32_t height) = 0;


	virtual ~IImage() = default;
};

class Image : public IImage
{
public:
	Image(const std::string& path, uint32_t width, uint32_t height)
		: m_height(height)
		, m_width(width)
		, m_path(path)
	{
		if (width < 1 || width > 10000 || height < 1 || height > 10000)
		{
			throw std::invalid_argument("Size must be in range from 1 to 10000");
		}
	}

	std::string GetFileName() const override
	{
		std::filesystem::path p(m_path);

		return p.filename().string();
	}

	std::string GetPath() const override
	{
		return m_path;
	}

	int GetWidth() const override
	{
		return m_width;
	}

	int GetHeight() const override
	{
		return m_height;
	}

	void Resize(uint32_t width, uint32_t height) override
	{
		m_width = width;
		m_height = height;
	}

private:
	std::string m_path;
	uint32_t m_width;
	uint32_t m_height;
};