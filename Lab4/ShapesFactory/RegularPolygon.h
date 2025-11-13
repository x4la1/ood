#pragma once
#include "Shape.h"
#include <numbers>

class RegularPolygon : public Shape
{
public:
	RegularPolygon(uint32_t vertexCount, const Point& center, double radius, Color color)
		: Shape(color)
		, m_vertexCount{ vertexCount }
		, m_center{ center }
		, m_radius{ radius }
	{
		if (vertexCount < 3)
		{
			throw std::invalid_argument("Vertexes count must be more than 2\n");
		}

		if (radius < 0.0)
		{
			throw std::invalid_argument("Radius must be positive\n");
		}
	}

	void Draw(ICanvas& canvas) override
	{
		canvas.SetColor(GetColor());
		std::vector<Point> vertices = GetCalculatedVertices();

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			size_t next = (i + 1) % vertices.size();
			canvas.DrawLine(vertices[i], vertices[next]);
		}
	}

	uint32_t GetVertexCount() const { return m_vertexCount; }
	Point GetCenter() const { return m_center; }
	double GetRadius() const { return m_radius; }

private:
	std::vector<Point> GetCalculatedVertices() const
	{
		std::vector<Point> vertices;
		double angleStep = 2 * std::numbers::pi / m_vertexCount;
		double startAngle = std::numbers::pi / 2;

		for (uint32_t i = 0; i < m_vertexCount; ++i)
		{
			double angle = startAngle + i * angleStep;
			double x = m_center.x + m_radius * std::cos(angle);
			double y = m_center.y + m_radius * std::sin(angle);
			vertices.push_back({ x, y });
		}

		return vertices;
	}

	uint32_t m_vertexCount;
	Point m_center;
	double m_radius;
};