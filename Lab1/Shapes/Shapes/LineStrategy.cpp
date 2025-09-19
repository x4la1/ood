#pragma once
#include "LineStrategy.h"

shapes::LineStrategy::LineStrategy(const Point& start, const Point& end)
	: m_start{ start }
	, m_end{ end }
{
}

void shapes::LineStrategy::Draw(gfx::ICanvas& canvas, const gfx::Color& color)
{
	canvas.SetColor(color);
	canvas.MoveTo(m_start.GetX(), m_start.GetY());
	canvas.LineTo(m_end.GetX(), m_end.GetY());
}
void shapes::LineStrategy::Move(const Point& vector)
{
	m_start += vector;
	m_end += vector;
}

std::string shapes::LineStrategy::ParametersToString() const
{
	return std::to_string(m_start.GetX()) + " "
		+ std::to_string(m_start.GetY()) + " "
		+ std::to_string(m_end.GetX()) + " "
		= std::to_string(m_end.GetX());
}

std::string shapes::LineStrategy::GetShapeName() const
{
	return "line";
}
