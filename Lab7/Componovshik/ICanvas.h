#pragma once
#include "Common.h"
#include "SFML/Graphics.hpp"
#include <numbers>
#include <optional>
#include <vector>

class ICanvas
{
public:
	virtual void DrawLine(Point from, Point to) = 0;
	virtual void DrawEllipse(Point center, float width, float height) = 0;
	virtual void FillEllipse(Point center, float width, float height) = 0;
	virtual void FillPolygon(const std::vector<Point>& points) = 0;
	virtual void SetOutlineColor(uint32_t color) = 0;
	virtual void SetFillColor(uint32_t color) = 0;
	virtual void SetOutlineThickness(float thickness) = 0;

	virtual ~ICanvas() = default;
};

class SFMLCanvas : public ICanvas
{
public:
	explicit SFMLCanvas(sf::RenderTarget& target)
		: m_target(target)
		, m_outlineColor(sf::Color::Black)
		, m_fillColor(sf::Color::White)
		, m_outlineThickness(1.0f)
	{
	}

	void DrawLine(Point from, Point to) override
	{
		sf::Vector2f direction(to.x - from.x, to.y - from.y);
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		direction /= length;

		float angle = std::atan2(direction.y, direction.x) * 180.0f / std::numbers::pi;

		sf::RectangleShape line(sf::Vector2f(length, m_outlineThickness));
		line.setFillColor(sf::Color::Black);
		line.setOrigin(0, m_outlineThickness / 2.0f);
		line.setPosition(from.x, from.y);
		line.setRotation(angle);

		m_target.draw(line);
	}

	void DrawEllipse(Point center, float width, float height) override
	{
		const int segments = 60;
		float rx = width / 2.0f;
		float ry = height / 2.0f;

		sf::ConvexShape ellipse;
		ellipse.setPointCount(segments);
		ellipse.setFillColor(sf::Color::Transparent);
		ellipse.setOutlineColor(m_outlineColor);
		ellipse.setOutlineThickness(m_outlineThickness);

		for (int i = 0; i < segments; ++i)
		{
			float angle = 2 * std::numbers::pi * i / segments;
			float x = center.x + rx * std::cos(angle);
			float y = center.y + ry * std::sin(angle);
			ellipse.setPoint(i, sf::Vector2f(x, y));
		}

		m_target.draw(ellipse);
	}

	void FillEllipse(Point center, float width, float height) override
	{
		const int segments = 60;
		float rx = width / 2.0f;
		float ry = height / 2.0f;

		sf::ConvexShape ellipse;
		ellipse.setPointCount(segments);
		ellipse.setFillColor(m_fillColor);
		ellipse.setOutlineThickness(0);

		for (int i = 0; i < segments; ++i)
		{
			float angle = 2 * std::numbers::pi * i / segments;
			float x = center.x + rx * std::cos(angle);
			float y = center.y + ry * std::sin(angle);
			ellipse.setPoint(i, sf::Vector2f(x, y));
		}

		m_target.draw(ellipse);
	}

	void FillPolygon(const std::vector<Point>& points) override
	{
		if (points.size() < 3)
			return;

		sf::ConvexShape polygon;
		polygon.setPointCount(points.size());
		polygon.setFillColor(m_fillColor);
		polygon.setOutlineThickness(0);

		for (size_t i = 0; i < points.size(); ++i)
		{
			polygon.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
		}

		m_target.draw(polygon);
	}

	void SetOutlineColor(uint32_t color) override
	{
		m_outlineColor = ToColor(color);
	}

	void SetFillColor(uint32_t color) override
	{
		m_fillColor = ToColor(color);
	}

	void SetOutlineThickness(float thickness) override
	{
		m_outlineThickness = thickness;
	}

private:
	static sf::Color ToColor(uint32_t color)
	{
		sf::Uint8 r = (color >> 24) & 0xFF;
		sf::Uint8 g = (color >> 16) & 0xFF;
		sf::Uint8 b = (color >> 8) & 0xFF;
		sf::Uint8 a = color & 0xFF;
		return sf::Color(r, g, b, a);
	}

	sf::RenderTarget& m_target;
	sf::Color m_outlineColor;
	sf::Color m_fillColor;
	float m_outlineThickness;
};