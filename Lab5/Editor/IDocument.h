#pragma once
#include <memory>
#include <string>
#include <optional>
#include <fstream>
#include "DocumentItem.h"

namespace fs = std::filesystem;

class IDocument
{
public:
	virtual void InsertParagraph(const std::string& text,
		std::optional<size_t> position)
		= 0;
	virtual void InsertImage(const std::string& path, uint32_t width, uint32_t height,
		std::optional<size_t> position)
		= 0;

	virtual size_t GetItemsCount() const = 0;
	virtual DocumentItem& GetItem(size_t index) = 0;

	virtual void DeleteItem(size_t index) = 0;

	virtual void SetTitle(const std::string& title) = 0;
	virtual std::string GetTitle() const = 0;

	virtual void Save(const std::string& path) = 0;

	virtual void List(std::ostream& output) const = 0;

	virtual ~IDocument() = default;
};

class HtmlDocument : public IDocument
{
public:
	void InsertParagraph(const std::string& text,
		std::optional<size_t> index) override
	{
		if (index == std::nullopt)
		{
			index = m_items.size();
		}

		if (!IsValidInsertPosition(index.value()))
		{
			throw std::invalid_argument("Invalid position");
		}

		auto paragraph = std::make_shared<Paragraph>(text);
		DocumentItem item(paragraph);

		InsertDocumentItem(item, index.value());
	}

	void InsertImage(const std::string& path, uint32_t width, uint32_t height,
		std::optional<size_t> index) override
	{
		if (index == std::nullopt)
		{
			index = m_items.size();
		}

		if (!IsValidInsertPosition(index.value()))
		{
			throw std::invalid_argument("Invalid position");
		}

		auto image = std::make_shared<Image>(path, width, height);
		DocumentItem item(image);

		InsertDocumentItem(item, index.value());
	}

	size_t GetItemsCount() const override
	{
		return m_items.size();
	}

	DocumentItem& GetItem(size_t index) override
	{
		if (!IsValidPosition(index))
		{
			throw std::invalid_argument("Invalid position");
		}

		return m_items[index];
	}

	void DeleteItem(size_t index) override
	{
		if (!IsValidPosition(index))
		{
			throw std::invalid_argument("Invalid position");
		}

		m_items.erase(m_items.begin() + index);
	}

	void SetTitle(const std::string& title) override
	{
		m_title = title;
	}

	std::string GetTitle() const override
	{
		return m_title;
	}

	void Save(const std::string& path) override
	{
		fs::path htmlPath(path);
		fs::path targetDir = htmlPath.parent_path();
		fs::path imagesSubDir = "images";
		fs::path fullImagesDir = targetDir / imagesSubDir;

		try
		{
			if (!m_items.empty())
			{
				fs::create_directories(fullImagesDir);
			}

			std::ofstream outFile(htmlPath);
			if (!outFile.is_open())
			{
				throw std::runtime_error("Failed to open file");
			}

			outFile << "<!DOCTYPE html>\n<html>\n<head>\n";
			outFile << "  <title>" << EscapeHtml(m_title) << "</title>\n";
			outFile << "</head>\n<body>\n";

			for (const auto& item : m_items)
			{
				if (auto paragraph = item.GetParagraph())
				{
					outFile << "  <p>" << EscapeHtml(paragraph->GetText()) << "</p>\n";
				}
				else if (auto image = item.GetImage())
				{
					fs::path srcPath = image->GetPath();
					fs::path fileName = image->GetFileName();
					fs::path dstPath = fullImagesDir / fileName;

					fs::copy_file(srcPath, dstPath, fs::copy_options::overwrite_existing);

					outFile << "  <img src=\"" << (imagesSubDir / fileName).string() << "\" ";
					outFile << "width=\"" << image->GetWidth() << "\" ";
					outFile << "height=\"" << image->GetHeight() << "\" />\n";
				}
			}

			outFile << "</body>\n</html>";
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error(std::string("Error during saving: ") + e.what());
		}
	}

	void List(std::ostream& output) const override
	{
		output << "Title: " << m_title << "\n";
		uint32_t index = 1;
		for (auto& item : m_items)
		{
			output << index << ". " << DocumentItemToString(item) << "\n";
			++index;
		}

		std::cout << "\n";
	}

	static std::string DocumentItemToString(const DocumentItem& item)
	{
		if (auto paragraph = item.GetParagraph())
		{
			return "Paragraph: " + paragraph->GetText();
		}
		else if (auto image = item.GetImage())
		{
			return "Image: " + std::to_string(image->GetWidth()) + " " + std::to_string(image->GetHeight())
				+ " " + "images/" + image->GetFileName();
		}

		return "";
	}

	static std::string EscapeHtml(const std::string& data)
	{
		std::string buffer;
		buffer.reserve(data.size());
		for (size_t pos = 0; pos != data.size(); ++pos)
		{
			switch (data[pos])
			{
			case '&':
				buffer.append("&amp;");
				break;
			case '\"':
				buffer.append("&quot;");
				break;
			case '\'':
				buffer.append("&apos;");
				break;
			case '<':
				buffer.append("&lt;");
				break;
			case '>':
				buffer.append("&gt;");
				break;
			default:
				buffer.append(1, data[pos]);
				break;
			}
		}
		return buffer;
	}

private:
	std::string m_title = "Title";
	std::vector<DocumentItem> m_items;

	void InsertDocumentItem(DocumentItem& item, size_t pos)
	{
		if (pos == m_items.size())
		{
			m_items.emplace_back(item);
		}
		else
		{
			m_items.insert(m_items.begin() + pos, item);
		}
	}

	bool IsValidInsertPosition(size_t pos)
	{
		if (pos > m_items.size())
		{
			return false;
		}

		return true;
	}

	bool IsValidPosition(size_t pos)
	{
		if (pos >= m_items.size())
		{
			return false;
		}

		return true;
	}
};