#pragma once
#include "TriangleStrategy.h"

shapes::TriangleStrategy::TriangleStrategy(const Point& point1, const Point& point2, const Point& point3)
{
	m_point1 = point1;
}

void shapes::TriangleStrategy::Draw(gfx::ICanvas& canvas, const gfx::Color& color)
{
	canvas.SetColor(color);
	canvas.MoveTo(m_point1.GetX(), m_point1.GetY());
	canvas.LineTo(m_point2.GetX(), m_point2.GetY());
	canvas.LineTo(m_point3.GetX(), m_point3.GetY());
	canvas.LineTo(m_point1.GetX(), m_point1.GetY());
}

void shapes::TriangleStrategy::Move(const Point& vector)
{
	m_point1 += vector;
	m_point2 += vector;
	m_point3 += vector;
}

std::string shapes::TriangleStrategy::ParametersToString() const
{
	return std::to_string(m_point1.GetX()) + " "
		+ std::to_string(m_point1.GetY()) + " "
		+ std::to_string(m_point2.GetX()) + " "
		+ std::to_string(m_point2.GetY()) + " "
		+ std::to_string(m_point3.GetX()) + " "
		+ std::to_string(m_point3.GetY());
}

std::string shapes::TriangleStrategy::GetShapeName() const
{
	return "triangle";
}