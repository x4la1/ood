#include "Color.h"
#include <stdexcept>
#include <string>
#include <iomanip>
#include <sstream>

gfx::Color::Color()
	: m_color{ 0x000000 }
{
}

gfx::Color::Color(const uint32_t& color)
{
	if (color > 0xFFFFFF)
	{
		throw std::invalid_argument("Color must be in RGB format \n");
	}

	m_color = color;
}

uint32_t gfx::Color::GetRgbaColor() const
{
	return (m_color << 8) | 0xFF;
}

std::string gfx::Color::ToString() const
{
	std::stringstream ss;
	ss << '#' << std::hex << std::setw(6) << std::setfill('0') << (m_color & 0xFFFFFF);
	
	return ss.str();
	
}