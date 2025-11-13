#pragma once
#include "Canvas.h"
#include <math.h>

class Shape
{
public:
	Shape(const Color& color)
		: m_color{ color }
	{
	}

	Color GetColor() const
	{
		return m_color;
	}

	virtual void Draw(ICanvas& canvas) = 0;

private:
	Color m_color;
};