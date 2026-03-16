#pragma once
#include <cstdint>
#include <optional>
#include <stdexcept>

class FillStyle
{
public:
	FillStyle(std::optional<uint32_t> color)
		: m_color(color)
	{
	}

	std::optional<uint32_t> GetFillColor() const
	{
		return m_color;
	}

	void SetFillColor(std::optional<uint32_t> color)
	{
		m_color = color;
	}

	bool IsEnable()
	{
		if (!m_color)
		{
			return false;
		}

		return true;
	}

private:
	std::optional<uint32_t> m_color = std::nullopt;
};

class OutlineStyle
{
public:
	OutlineStyle(std::optional<uint32_t> color, std::optional<float> thickness)
		: m_color(color)
		, m_thickness(thickness)
	{
		if (thickness.has_value() && thickness.value() < 0)
		{
			throw std::invalid_argument("Thickness must be positive");
		}
	}

	std::optional<uint32_t> GetOutlineColor() const
	{
		return m_color;
	}

	void SetOutlineColor(std::optional<uint32_t> color)
	{
		m_color = color;
	}

	std::optional<float> GetThickness() const
	{
		return m_thickness;
	}

	void SetThickness(std::optional<float> thickness)
	{
		m_thickness = thickness;
	}

	bool IsEnable()
	{
		if (!m_color || !m_thickness)
		{
			return false;
		}

		return true;
	}

private:
	std::optional<uint32_t> m_color = std::nullopt;
	std::optional<float> m_thickness = std::nullopt;
};