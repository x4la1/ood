#pragma once
#include "Color.h"
#include "ICanvas.h"
#include "Point.h"

namespace shapes
{

class IShapeStrategy
{
public:
	virtual void Draw(gfx::ICanvas& canvas, const gfx::Color& color) = 0;
	virtual void Move(const Point& vector) = 0;
	virtual std::string ParametersToString() const = 0;
	virtual std::string GetShapeName() const = 0;
	virtual ~IShapeStrategy() = default;
};
} // namespace shapes