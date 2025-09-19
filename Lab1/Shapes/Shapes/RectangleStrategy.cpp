#pragma once
#include "RectangleStrategy.h"
#include <stdexcept>

shapes::RectangleStrategy::RectangleStrategy(const Point& leftTop, const double& width, const double& height)
{
	if (width < 0.0 || height < 0.0)
	{
		throw std::invalid_argument("Width and height must be positive\n");
	}
	m_leftTop = leftTop;
	m_heigth = height;
	m_width = width;
}

void shapes::RectangleStrategy::Draw(gfx::ICanvas& canvas, const gfx::Color& color)
{
	canvas.SetColor(color);
	canvas.MoveTo(m_leftTop.GetX(), m_leftTop.GetY());
	canvas.LineTo(m_leftTop.GetX() + m_width, m_leftTop.GetY());
	canvas.LineTo(m_leftTop.GetX() + m_width, m_leftTop.GetY() + m_heigth);
	canvas.LineTo(m_leftTop.GetX(), m_leftTop.GetY() + m_heigth);
	canvas.LineTo(m_leftTop.GetX(), m_leftTop.GetY());
}

void shapes::RectangleStrategy::Move(const Point& vector)
{
	m_leftTop += vector;
}

std::string shapes::RectangleStrategy::ParametersToString() const
{
	return std::to_string(m_leftTop.GetY()) + " "
		+ std::to_string(m_leftTop.GetY()) + " "
		+ std::to_string(m_heigth) + " "
		+ std::to_string(m_width);
}

std::string shapes::RectangleStrategy::GetShapeName() const
{
	return "rectangle";
}