#pragma once
#include "IShapeStrategy.h"
#include "Point.h"

namespace shapes
{

class LineStrategy : public shapes::IShapeStrategy
{
public:
	LineStrategy(const Point& start, const Point& end);
	void Draw(gfx::ICanvas& canvas, const gfx::Color& color) override;
	void Move(const Point& vector) override;
	std::string ParametersToString() const override;
	std::string GetShapeName() const override;

private:
	Point m_start;
	Point m_end;
};

} // namespace shapes