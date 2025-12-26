#pragma once
#include "GraphicsLib.h"
#include "ModernGraphicsLib.h"

class ModernObjectAdapter : public graphics_lib::ICanvas
{
public:
	ModernObjectAdapter(modern_graphics_lib::ModernGraphicsRenderer& renderer)
		: m_renderer{ renderer }
	{
		m_renderer.BeginDraw(); //logic клиент должен вызвать begin и end
	}

	~ModernObjectAdapter() override
	{
		try
		{
			m_renderer.EndDraw();
		}
		catch (std::logic_error&)
		{
		}
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
		m_renderer.DrawLine(m_currentPoint, { x, y }, RgbToRgba(m_color));
		m_currentPoint = { x, y };
	}

	modern_graphics_lib::RGBAColor RgbToRgba(uint32_t color)
	{
		uint32_t r, g, b;
		r = (color >> 16) & 0xFF;
		g = (color >> 8) & 0xFF;
		b = color & 0xFF;

		return modern_graphics_lib::RGBAColor{ r / 255.0f, g / 255.0f, b / 255.0f, 1.0f };
	}

private:
	uint32_t m_color = 0x000000;
	modern_graphics_lib::Point m_currentPoint{ 0, 0 };
	modern_graphics_lib::ModernGraphicsRenderer& m_renderer;
};