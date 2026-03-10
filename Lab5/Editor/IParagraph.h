#pragma once
#include <string>

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