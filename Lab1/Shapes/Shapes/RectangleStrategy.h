#pragma once
#include "IShapeStrategy.h"
#include "Point.h"

namespace shapes
{

class RectangleStrategy : public shapes::IShapeStrategy
{
public:
	RectangleStrategy(const Point& leftTop, const double& width, const double& height);
	void Draw(gfx::ICanvas& canvas, const gfx::Color& color) override;
	void Move(const Point& vector) override;
	std::string ParametersToString() const override;
	virtual std::string GetShapeName() const override;

private:
	Point m_leftTop;
	double m_heigth;
	double m_width;
};

} // namespace shapes