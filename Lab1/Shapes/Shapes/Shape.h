#pragma once
#include "Color.h"
#include "ICanvas.h"
#include "IShapeStrategy.h"
#include "Point.h"
#include <memory>

namespace shapes
{

class Shape
{
public:
	Shape(std::unique_ptr<IShapeStrategy> shapeStrategy, const gfx::Color& color);
	void SetStrategy(std::unique_ptr<IShapeStrategy> shapeStrategy);
	void Draw(gfx::ICanvas& canvas) const;
	void MoveShape(const Point& vector);
	void ChangeColor(const gfx::Color& color);
	std::string ParametersToString() const;
	std::string GetShapeName() const;
	gfx::Color GetColor() const;

private:
	gfx::Color m_color;
	std::unique_ptr<IShapeStrategy> m_shapeStrategy;
};
} // namespace shapes
