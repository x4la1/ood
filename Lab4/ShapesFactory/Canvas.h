#include "EllipseShape.h"
#include "SFML/Graphics.hpp"
#include "Types.h"

class ICanvas
{
public:
	virtual void SetColor(Color color) = 0;
	virtual void DrawLine(Point from, Point to) const = 0;
	virtual void DrawEllipse(Point center, double verticalRadius, double horizontalRadius) const = 0;
};

class Canvas : public ICanvas
{
public:
	Canvas(sf::RenderWindow& window)
		: m_window(window)
		, m_color(Color::Black)
	{
	}

	void SetColor(Color color) override
	{
		m_color = color;
	}

	void DrawLine(Point from, Point to) const override
	{
		
		sf::Vector2f start(static_cast<float>(from.x), static_cast<float>(from.y));
		sf::Vector2f end(static_cast<float>(to.x), static_cast<float>(to.y));

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
		line.setFillColor(sf::Color(ColorToRgba()));

		m_window.draw(line);
	}

	void DrawEllipse(Point center, double rx, double ry) const override
	{
		EllipseShape ellipse(sf::Vector2f(static_cast<float>(rx), static_cast<float>(ry)));
		ellipse.setPosition(static_cast<float>(center.x - rx), static_cast<float>(center.y - ry));
		ellipse.setOutlineColor(sf::Color(ColorToRgba()));
		ellipse.setOutlineThickness(3.f);
		ellipse.setFillColor(sf::Color::Transparent);

		m_window.draw(ellipse);
	}

private:
	uint32_t ColorToRgba() const
	{
		switch (m_color)
		{
		case Color::Green:
			return 0x00ff00ff;
		case Color::Red:
			return 0xff0000ff;
		case Color::Blue:
			return 0x0000ffff;
		case Color::Yellow:
			return 0xffff00ff;
		case Color::Pink:
			return 0xff00ffff;
		case Color::Black:
			return 0x000000ff;
		};
	}

	sf::RenderWindow& m_window;
	Color m_color;
};