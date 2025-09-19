#pragma once
#include "Canvas.h"
#include "EllipseShape.h"

gfx::Canvas::Canvas(sf::RenderTexture& window)
	: m_window{ window }
	, m_currentPoint{ Point(0.0, 0.0) }
	, m_color{ 0x000000 }
{
	m_font.loadFromFile("arial.ttf");
}

void gfx::Canvas::MoveTo(const double& x, const double& y)
{
	m_currentPoint = Point(x, y);
}

void gfx::Canvas::SetColor(const gfx::Color& color)
{
	m_color = color;
}

void gfx::Canvas::LineTo(const double& x, const double& y)
{
	sf::Vector2f start(m_currentPoint.GetX(), m_currentPoint.GetY());
	sf::Vector2f end(x, y);

	sf::Vector2f direction = end - start;
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	direction /= length;
	sf::Vector2f perpendicular(-direction.y, direction.x);
	perpendicular *= 2.0f;

	sf::ConvexShape line(4);
	line.setPoint(0, start + perpendicular);
	line.setPoint(1, start);
	line.setPoint(2, end);
	line.setPoint(3, end + perpendicular);
	line.setFillColor(sf::Color(m_color.GetRgbaColor()));

	m_currentPoint = Point(x, y);

	m_window.draw(line);
}

void gfx::Canvas::DrawEllipse(const double& cx, const double& cy, const double& rx, const double& ry) const
{
	EllipseShape ellipse(sf::Vector2f(rx, ry));
	ellipse.setPosition(cx - rx, cy - ry);
	ellipse.setOutlineColor(sf::Color(m_color.GetRgbaColor()));
	ellipse.setOutlineThickness(3.f);
	ellipse.setFillColor(sf::Color::Transparent);

	m_window.draw(ellipse);
}

void gfx::Canvas::DrawText(const double& left, const double& top, const double& fontSize, const std::string& text) const
{
	if (fontSize < 0.0)
	{
		throw std::invalid_argument("Font size must be positive\n");
	}

	sf::Text drawableText;
	drawableText.setString(text);
	drawableText.setPosition(left, top);
	drawableText.setFont(m_font);
	drawableText.setCharacterSize(static_cast<uint32_t>(fontSize));
	drawableText.setFillColor(sf::Color(m_color.GetRgbaColor()));

	m_window.draw(drawableText);
}
