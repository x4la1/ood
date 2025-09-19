#pragma once
#include "IShapeStrategy.h"
#include "Point.h"
#include <vector>

namespace shapes
{

class TriangleStrategy : public shapes::IShapeStrategy
{
public:
	TriangleStrategy(const Point& point1, const Point& point2, const Point& point3);
	void Draw(gfx::ICanvas& canvas, const gfx::Color& color) override;
	void Move(const Point& vector) override;
	std::string ParametersToString() const override;
	std::string GetShapeName() const override;

private:
	Point m_point1;
	Point m_point2;
	Point m_point3;
};
} // namespace shapes