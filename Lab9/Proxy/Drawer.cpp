#include "Drawer.h"
#include <cassert>
#include <cstdlib>

namespace
{

int Sign(int value)
{
	return (0 < value) - (value < 0);
}

void DrawSteepLine(Image& image, Point from, Point to, char color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	assert(deltaY >= deltaX);

	if (from.y > to.y)
	{
		std::swap(from, to);
	}

	const int stepX = Sign(to.x - from.x);
	const int errorThreshold = deltaY + 1;
	const int deltaErr = deltaX + 1;

	int error = deltaErr / 2;

	for (Point p = from; p.y <= to.y; ++p.y)
	{
		image.SetPixel({ p.x, p.y }, color);
		assert((p.y != to.y) || (p.x == to.x));

		error += deltaErr;

		if (error >= errorThreshold)
		{
			p.x += stepX;
			error -= errorThreshold;
		}
	}
}

void DrawSlopeLine(Image& image, Point from, Point to, char color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	assert(deltaX >= deltaY);

	if (from.x > to.x)
	{
		std::swap(from, to);
	}

	const int stepY = Sign(to.y - from.y);
	const int errorThreshold = deltaX + 1;
	const int deltaErr = deltaY + 1;

	int error = deltaErr / 2;

	for (Point p = from; p.x <= to.x; ++p.x)
	{
		image.SetPixel({ p.x, p.y }, color);
		assert((p.x != to.x) || (p.y == to.y));

		error += deltaErr;

		if (error >= errorThreshold)
		{
			p.y += stepY;
			error -= errorThreshold;
		}
	}
}

void DrawPixelsForCircle(Image& image, int x, int y, Point center, char color)
{
	image.SetPixel({ center.x + x, center.y + y }, color);
	image.SetPixel({ center.x + x, center.y - y }, color);
	image.SetPixel({ center.x - x, center.y + y }, color);
	image.SetPixel({ center.x - x, center.y - y }, color);
	image.SetPixel({ center.x + y, center.y + x }, color);
	image.SetPixel({ center.x + y, center.y - x }, color);
	image.SetPixel({ center.x - y, center.y + x }, color);
	image.SetPixel({ center.x - y, center.y - x }, color);
}

void DrawLineForCircle(Image& image, int x, int y, Point center, char color)
{
	DrawLine(image, { center.x - x, center.y + y }, { center.x + x, center.y + y }, color);
	DrawLine(image, { center.x - y, center.y + x }, { center.x + y, center.y + x }, color);
	DrawLine(image, { center.x - x, center.y - y }, { center.x + x, center.y - y }, color);
	DrawLine(image, { center.x - y, center.y - x }, { center.x + y, center.y - x }, color);
}
} // namespace

void DrawLine(Image& image, Point from, Point to, char color)
{
	const int deltaX = std::abs(to.x - from.x);
	const int deltaY = std::abs(to.y - from.y);

	if (deltaY > deltaX)
	{
		DrawSteepLine(image, from, to, color);
	}
	else
	{
		DrawSlopeLine(image, from, to, color);
	}
}

void DrawCircle(Image& image, Point center, int radius, char color)
{
	if (radius < 0)
	{
		return;
	}

	if (radius == 0)
	{
		image.SetPixel(center, color);
		return;
	}

	int x = 0;
	int y = radius;
	int delta = 3 - 2 * radius;
	DrawPixelsForCircle(image, x, y, center, color);
	while (y >= x)
	{
		if (delta <= 0)
		{
			delta += 4 * x + 6;
		}
		else
		{
			--y;
			delta += 4 * (x - y) + 10;
		}
		++x;
		DrawPixelsForCircle(image, x, y, center, color);
	}
}

void FillCircle(Image& image, Point center, int radius, char color)
{
	if (radius < 0)
	{
		return;
	}

	if (radius == 0)
	{
		image.SetPixel(center, color);
		return;
	}

	int x = 0;
	int y = radius;
	int delta = 3 - 2 * radius;
	DrawLineForCircle(image, x, y, center, color);
	while (y >= x)
	{
		if (delta <= 0)
		{
			delta += 4 * x + 6;
		}
		else
		{
			--y;
			delta += 4 * (x - y) + 10;
		}
		++x;
		DrawLineForCircle(image, x, y, center, color);
	}
}