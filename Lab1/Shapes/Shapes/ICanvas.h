#pragma once
#include "Color.h"
#include "Point.h"
#include <string>

namespace gfx
{
class ICanvas
{
public:
	virtual void MoveTo(const double& x, const double& y) = 0;
	virtual void SetColor(const Color& color) = 0;
	virtual void LineTo(const double& x, const double& y) = 0;
	virtual void DrawEllipse(const double& cx, const double& cy, const double& xr, const double& ry) const = 0;
	virtual void DrawText(const double& left, const double& top, const double& fontSize, const std::string& text) const = 0;
	virtual ~ICanvas() = default;
};
} // namespace gfx