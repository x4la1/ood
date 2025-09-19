#include "CircleStrategy.h"
#include <stdexcept>

shapes::CircleStrategy::CircleStrategy(const Point& center, const double& radius)
{
	if (radius < 0.0)
	{
		throw std::invalid_argument("Radius must be positive\n");
	}

	m_center = center;
	m_radius = radius;
}

void shapes::CircleStrategy::Draw(gfx::ICanvas& canvas, const gfx::Color& color)
{
	canvas.SetColor(color);
	canvas.DrawEllipse(m_center.GetX(), m_center.GetY(), m_radius, m_radius);
}

void shapes::CircleStrategy::Move(const Point& vector)
{
	m_center += vector;
}

std::string shapes::CircleStrategy::ParametersToString() const
{
	return std::to_string(m_center.GetX()) + " "
		+ std::to_string(m_center.GetY()) + " "
		+ std::to_string(m_radius);
}

std::string shapes::CircleStrategy::GetShapeName() const
{
	return "circle";
}