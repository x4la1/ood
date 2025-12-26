#pragma once
#include "geom.h"
#include <array>
#include <cassert>
#include <optional>

class Tile
{
public:
	constexpr static int SIZE = 8;

	Tile(char color = ' ') noexcept
	{
		for (auto& row : m_pixels)
		{
			std::fill(row.begin(), row.end(), color);
		}

		assert(m_instanceCount >= 0);
		++m_instanceCount;
	}

	Tile(const Tile& other)
		: m_pixels{ other.m_pixels }
	{

		assert(m_instanceCount >= 0);
		++m_instanceCount;
	}

	~Tile()
	{
		--m_instanceCount;
		assert(m_instanceCount >= 0);
	}

	void SetPixel(Point p, char color) noexcept
	{
		if (IsPointInSize(p, { SIZE, SIZE }))
		{
			m_pixels[p.y][p.x] = color;
		}
	}

	char GetPixel(Point p) const noexcept
	{
		if (IsPointInSize(p, { SIZE, SIZE }))
		{
			return m_pixels[p.y][p.x];
		}
		return ' ';
	}

	static int GetInstanceCount() noexcept
	{
		return m_instanceCount;
	}

private:
	inline static int m_instanceCount = 0;

	std::array<std::array<char, SIZE>, SIZE> m_pixels{};
};