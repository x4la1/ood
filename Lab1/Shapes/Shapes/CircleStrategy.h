#pragma once
#include "IShapeStrategy.h"
#include "Point.h"
namespace shapes
{

class CircleStrategy : public shapes::IShapeStrategy
{
public:
	CircleStrategy(const Point& center, const double& radius);
	void Draw(gfx::ICanvas& canvas, const gfx::Color& color) override;
	void Move(const Point& vector) override;
	std::string ParametersToString() const override;
	std::string GetShapeName() const override;

private:
	Point m_center = Point(0, 0);
	double m_radius;
};
} // namespace shapes