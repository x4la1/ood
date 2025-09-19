#pragma once
#include <SFML/Graphics.hpp>

class EllipseShape : public sf::Shape
{
public:
	explicit EllipseShape(const sf::Vector2f& radius = { 0.f, 0.f });
	void setRadius(const sf::Vector2f& radius);
	const sf::Vector2f& getRadius() const;
	std::size_t getPointCount() const override;
	sf::Vector2f getPoint(std::size_t index) const override;

private:
	static const std::size_t s_pointCount = 30;
	sf::Vector2f m_radius;
};