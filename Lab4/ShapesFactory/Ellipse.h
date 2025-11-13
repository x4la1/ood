#pragma once
#include "Shape.h"

class Ellipse : public Shape
{
public:
	Ellipse(const Point& center, const double& rx, const double& ry, const Color& color)
		: Shape{ color }
		, m_center{ center }
		, m_rx{ rx }
		, m_ry{ ry }
	{
		if (rx < 0.0 || ry < 0.0)
		{
			throw std::invalid_argument("Radius must be positive\n");
		}
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		canvas.DrawEllipse(m_center, m_rx, m_ry);
	}

	Point GetCenter() const { return m_center; }
	double GetHorizontalRadius() const { return m_rx; }
	double GetverticalRadius() const { return m_ry; }

private:
	Point m_center;
	double m_rx;
	double m_ry;
};