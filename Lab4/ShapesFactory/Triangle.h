#pragma once
#include "Shape.h"

class Triangle : public Shape
{
public:
	Triangle(const Point& vertex1, const Point& vertex2, const Point& vertex3, Color color)
		: Shape{ color }
		, m_vertex1{ vertex1 }
		, m_vertex2{ vertex2 }
		, m_vertex3{ vertex3 }
	{
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		canvas.DrawLine(m_vertex1, m_vertex2);
		canvas.DrawLine(m_vertex2, m_vertex3);
		canvas.DrawLine(m_vertex3, m_vertex1);
	}

	Point GetVertex1() { return m_vertex1; }
	Point GetVertex2() { return m_vertex2; }
	Point GetVertex3() { return m_vertex3; }

private:
	Point m_vertex1;
	Point m_vertex2;
	Point m_vertex3;
};