#include "Image.h"
#include <ostream>
#include <sstream>
#include <stdexcept>

/**
 * Конструирует изображение заданного размера. Если размеры не являются положительными,
 * выбрасывает исключение std::out_of_range.
 */
Image::Image(Size size, char color)
{
	if (size.width <= 0 || size.height <= 0)
	{
		throw std::out_of_range("Image dimensions must be positive");
	}

	m_size = size;

	int tilesWidth = GetTileCount(size.width);
	int tilesHeight = GetTileCount(size.height);

	CoW<Tile> initialTile(color);

	m_tiles.resize(tilesHeight);
	for (auto& row : m_tiles)
	{
		row.assign(tilesWidth, initialTile);
	}
}

// Возвращает размер изображения в пикселях.
Size Image::GetSize() const noexcept
{
	return m_size;
}

/**
 * Возвращает «цвет» пикселя в указанных координатах.Если координаты выходят за пределы
 * изображения, возвращает «пробел».
 */
char Image::GetPixel(Point p) const noexcept
{
	if (!IsPointInSize(p, m_size))
	{
		return ' ';
	}

	const int tileX = p.x / Tile::SIZE;
	const int tileY = p.y / Tile::SIZE;
	const Point localPoint{ p.x % Tile::SIZE, p.y % Tile::SIZE };

	return m_tiles[tileY][tileX]->GetPixel(localPoint);
}

/**
 * Задаёт «цвет» пикселя в указанных координатах. Если координаты выходят за пределы изображения
 * действие игнорируется.
 */
void Image::SetPixel(Point p, char color) 
{
	if (!IsPointInSize(p, m_size))
	{
		return;
	}

	const int tileX = p.x / Tile::SIZE;
	const int tileY = p.y / Tile::SIZE;
	const Point localPoint{ p.x % Tile::SIZE, p.y % Tile::SIZE };


	m_tiles[tileY][tileX].Write()->SetPixel(localPoint, color);
}

int Image::GetTileCount(int dimension)
{
	return std::ceil(static_cast<double>(dimension) / Tile::SIZE);
}

/**
 * Выводит в поток out изображение в виде символов.
 */
void Print(const Image& img, std::ostream& out)
{
	const auto size = img.GetSize();
	for (int y = 0; y < size.height; ++y)
	{
		for (int x = 0; x < size.width; ++x)
		{
			out.put(img.GetPixel({ x, y }));
		}
		out.put('\n');
	}
}

/**
 * Загружает изображение из pixels. Линии изображения разделяются символами \n.
 * Размеры картинки определяются по количеству переводов строки и самой длинной линии.
 */
Image LoadImage(const std::string& pixels)
{
	std::istringstream s(pixels);
	Size size;
	std::string line;
	while (std::getline(s, line))
	{
		size.width = std::max(size.width, static_cast<int>(line.length()));
		++size.height;
	}

	Image img(size);

	s = std::istringstream(pixels);
	for (int y = 0; y < size.height; ++y)
	{
		if (!std::getline(s, line))
			break;

		int x = 0;
		for (char ch : line)
		{
			img.SetPixel({ x++, y }, ch);
		}
	}

	return img;
}