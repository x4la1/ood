#pragma once
#include "TextStrategy.h"
#include <stdexcept>

shapes::TextStrategy::TextStrategy(const Point leftTop, const std::string& text, const double& fontSize)
{
	if (fontSize < 0.0)
	{
		throw std::invalid_argument("Font size must be positive\n");
	}
	m_leftTop = leftTop;
	m_fontSize = fontSize;
	m_text = text;
}

void shapes::TextStrategy::Draw(gfx::ICanvas& canvas, const gfx::Color& color)
{
	canvas.SetColor(color);
	canvas.DrawText(m_leftTop.GetX(), m_leftTop.GetY(), m_fontSize, m_text);
}

void shapes::TextStrategy::Move(const Point& vector)
{
	m_leftTop += vector;
}

std::string shapes::TextStrategy::ParametersToString() const
{
	return std::to_string(m_leftTop.GetX()) + " "
		+ std::to_string(m_leftTop.GetY()) + " "
		+ std::to_string(m_fontSize) + " "
		+ m_text;
}

std::string shapes::TextStrategy::GetShapeName() const
{
	return "text";
}