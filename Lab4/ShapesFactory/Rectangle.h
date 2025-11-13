#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
public:
	Rectangle(const Point& leftTop, const Point& rightBottom, Color color)
		: Shape{ color }
		, m_leftTop{ leftTop }
		, m_rightBottom{ rightBottom }
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());

		Point rightTop = { m_rightBottom.x, m_leftTop.y };
		Point leftBottom = { m_leftTop.x, m_rightBottom.y };
		canvas.DrawLine(m_leftTop, rightTop);
		canvas.DrawLine(rightTop, m_rightBottom);
		canvas.DrawLine(m_rightBottom, leftBottom);
		canvas.DrawLine(leftBottom, m_leftTop);
	}

	Point GetLeftTop() { return m_leftTop; }
	Point GetrightBottom() { return m_rightBottom; }

private:
	Point m_leftTop;
	Point m_rightBottom;
};