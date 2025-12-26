#pragma once
#include "CoW.h"
#include "Geom.h"
#include "Tile.h"
#include <cmath>
#include <iosfwd>
#include <string>
#include <vector>

class Image
{
public:
	explicit Image(Size size, char color = ' ');

	Size GetSize() const noexcept;
	char GetPixel(Point p) const noexcept;
	void SetPixel(Point p, char color);

private:
	int GetTileCount(int dimension);

	Size m_size;
	std::vector<std::vector<CoW<Tile>>> m_tiles;
};

void Print(const Image& img, std::ostream& out);

Image LoadImage(const std::string& pixels);