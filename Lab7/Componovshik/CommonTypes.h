#pragma once

struct RectD
{

	RectD(double width, double height, double leftTopX, double leftTopY)
		: m_width{ width }
		, m_height{ height }
		, m_leftTopX{ leftTopX }
		, m_leftTopY{ leftTopY }
	{
	}

	double m_width;
	double m_height;
	double m_leftTopX;
	double m_leftTopY;
};
