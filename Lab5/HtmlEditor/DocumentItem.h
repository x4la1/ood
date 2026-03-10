#pragma once
#include <filesystem>

typedef std::filesystem::path Path;

class IImage
{
public:
	// Возвращает путь относительно каталога документа
	virtual Path GetPath() const = 0;

	// Ширина изображения в пикселях
	virtual int GetWidth() const = 0;
	// Высота изображения в пикселях
	virtual int GetHeight() const = 0;

	// Изменяет размер изображения
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

	Path GetPath() const override
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
	Path m_path;
	uint32_t m_width;
	uint32_t m_height;
};

class IParagraph
{
public:
	virtual std::string GetText() const = 0;
	virtual void SetText(const std::string& text) = 0;
	virtual ~IParagraph() = default;
};

class Paragraph : public IParagraph
{
public:
	Paragraph(const std::string& text)
		: m_text(text)
	{
	}

	std::string GetText() const override
	{
		return m_text;
	}

	void SetText(const std::string& text) override
	{
		m_text = text;
	}

private:
	std::string m_text;
};

class ConstDocumentItem
{
public:
	ConstDocumentItem(const std::shared_ptr<IParagraph>& paragraph)
		: m_paragraph(paragraph)
		, m_image(nullptr)
	{
	}

	ConstDocumentItem(const std::shared_ptr<IImage>& image)
		: m_paragraph(nullptr)
		, m_image(image)
	{
	}

	std::shared_ptr<const IImage> GetImage() const
	{
		return m_image;
	}

	std::shared_ptr<const IParagraph> GetParagraph() const
	{
		return m_paragraph;
	}

	void SetDelete()
	{
		m_isDeleted = true;
	}

	void SetUnDelete()
	{
		m_isDeleted = false;
	}

	bool IsDeleted() const
	{
		return m_isDeleted;
	}

	virtual ~ConstDocumentItem() = default;

protected:
	std::shared_ptr<IParagraph> m_paragraph;
	std::shared_ptr<IImage> m_image;
	bool m_isDeleted = false;
};

class DocumentItem : public ConstDocumentItem
{
public:
	DocumentItem(const std::shared_ptr<IParagraph>& paragraph)
		: ConstDocumentItem(paragraph)
	{
	}

	DocumentItem(const std::shared_ptr<IImage>& image)
		: ConstDocumentItem(image)
	{
	}

	std::shared_ptr<IImage> GetImage()
	{
		return m_image;
	}

	std::shared_ptr<IParagraph> GetParagraph()
	{
		return m_paragraph;
	}
};
