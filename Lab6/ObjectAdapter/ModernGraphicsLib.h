#pragma once
#include <format>
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace modern_graphics_lib
{

class RGBAColor
{
public:
	RGBAColor(float r, float g, float b, float a)
		: r(r)
		, g(g)
		, b(b)
		, a(a)
	{
	}
	float r, g, b, a;
};

class Point
{
public:
	Point(int x, int y)
		: x(x)
		, y(y)
	{
	}
	int x;
	int y;
};

// ����� ��� ������������ ��������� �������
class ModernGraphicsRenderer
{
public:
	ModernGraphicsRenderer(std::ostream& strm)
		: m_out(strm)
	{
	}

	~ModernGraphicsRenderer()
	{
		if (m_drawing) // ��������� ���������, ���� ��� ���� ������
		{
			EndDraw();
		}
	}

	// ���� ����� ������ ���� ������ � ������ ���������
	void BeginDraw()
	{
		if (m_drawing)
		{
			throw std::logic_error("Drawing has already begun");
		}
		m_out << "<draw>" << std::endl;
		m_drawing = true;
	}

	// ��������� ��������� �����
	void DrawLine(const Point& start, const Point& end, const RGBAColor& color)
	{
		if (!m_drawing)
		{
			throw std::logic_error("DrawLine is allowed between BeginDraw()/EndDraw() only");
		}
		std::string info = std::format(
			R"(<line fromX="{}" fromY="{}" toX="{}" toY="{}">
  <color r="{:.2f}" g="{:.2f}" b="{:.2f}" a="{:.2f}" />
</line>
)",
			start.x, start.y, end.x, end.y, color.r, color.g, color.b, color.a);

		m_out << info;
	}

	// ���� ����� ������ ���� ������ � ����� ���������
	void EndDraw()
	{
		if (!m_drawing)
		{
			throw std::logic_error("Drawing has not been started");
		}
		m_out << "</draw>" << std::endl;
		m_drawing = false;
	}

private:
	std::ostream& m_out;
	bool m_drawing = false;
};
} // namespace modern_graphics_lib