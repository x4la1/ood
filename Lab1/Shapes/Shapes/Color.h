#pragma once
#include <cstdint>
#include <string>

namespace gfx
{
class Color
{
public:
	Color();
	Color(const uint32_t& color);
	uint32_t GetRgbaColor() const;
	std::string ToString() const;

private:
	uint32_t m_color;
};
} // namespace gfx