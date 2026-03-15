#pragma once
#include <stdexcept>

struct Point
{
	float x;
	float y;
};

class Frame
{
public:
	Frame(Point topLeft, float width, float height)
		: m_topLeft(topLeft)
		, m_width(width)
		, m_height(height)
	{
		if (width < 1 || height < 1)
		{
			throw std::invalid_argument("Size must be positive");
		}
	}

	Point GetTopLeft() const
	{
		return m_topLeft;
	}

	float GetWidth() const
	{
		return m_width;
	}

	float GetHeight() const
	{
		return m_height;
	}

private:
	Point m_topLeft;
	float m_width;
	float m_height;
};
