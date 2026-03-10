#pragma once
#include <memory>
#include <string>
#include <filesystem>
#include "IParagraph.h"
#include "IImage.h"

class DocumentItem
{
public:
	DocumentItem(std::shared_ptr<IParagraph> paragraph)
		:m_paragraph(paragraph)
	{
	}

	DocumentItem(std::shared_ptr<IImage> image)
		:m_image(image)
	{
	}
	std::shared_ptr<IParagraph> GetParagraph() const
	{
		return m_paragraph;
	}

	std::shared_ptr<IImage> GetImage() const
	{
		return m_image;
	}

private:
	std::shared_ptr<IParagraph> m_paragraph;
	std::shared_ptr<IImage> m_image;
};