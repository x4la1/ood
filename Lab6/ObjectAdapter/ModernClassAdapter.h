#pragma once
#include "GraphicsLib.h"
#include "ModernGraphicsLib.h"

class ModernClassAdapter : public graphics_lib::ICanvas
	, private modern_graphics_lib::ModernGraphicsRenderer
{
public:
	ModernClassAdapter(std::ostream& strm)
		: modern_graphics_lib::ModernGraphicsRenderer(strm)
		, m_color(0x000000)
	{
		BeginDraw();
	}

	~ModernClassAdapter() override
	{
		EndDraw();
	}

	void SetColor(uint32_t rgbColor) override
	{
		m_color = rgbColor;
	}

	void MoveTo(int x, int y) override
	{
		m_currentPoint = { x, y };
	}

	void LineTo(int x, int y) override
	{
		DrawLine(m_currentPoint, { x, y }, RgbToRgba(m_color));
		m_currentPoint = { x, y };
	}

private:
	modern_graphics_lib::RGBAColor RgbToRgba(uint32_t color) //либо static, либо вынести
	{
		uint32_t r, g, b;
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;

		return modern_graphics_lib::RGBAColor{ r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
	}

	uint32_t m_color = 0x000000;
	modern_graphics_lib::Point m_currentPoint{ 0, 0 };
};