#pragma once
#include "Common.h"
#include "SFML/Graphics.hpp"
#include <vector>

class ICanvas
{
public:
	virtual void DrawLine(Point from, Point to) = 0;
	virtual void DrawEllipse(Point center, float width, float height) = 0;
	virtual void FillEllipse(Point center, float width, float height) = 0;
	virtual void FillPolygon(const std::vector<Point>& points) = 0;
	virtual void SetOutlineColor(uint32_t color) = 0;
	virtual void SetFillColor(uint32_t color) = 0;
	virtual void SetOutlineThickness(float thickness) = 0;

	virtual ~ICanvas() = default;
};