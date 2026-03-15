#pragma once
#include "Common.h"
#include "IDrawable.h"
#include "Style.h"
#include <memory>
#include <optional>

class IShape : public IDrawable
{
public:
	virtual void SetFrame(const Frame& frame) = 0;
	virtual std::optional<Frame> GetFrame() const = 0;

	virtual void SetOutlineColor(std::optional<uint32_t> color) = 0;
	virtual std::optional<uint32_t> GetOutlineColor() const = 0;

	virtual void SetFillColor(std::optional<uint32_t> color) = 0;
	virtual std::optional<uint32_t> GetFillColor() const = 0;

	virtual void SetOutlineThickness(std::optional<float> thickness) = 0;
	virtual std::optional<float> GetOutlineThickness() const = 0;

	virtual std::unique_ptr<IShape> Clone() = 0;

	virtual ~IShape() = default;
};

class AbstractShape : public IShape
{
public:
	AbstractShape(Frame frame, OutlineStyle outlineStyle, FillStyle fillStyle)
		: m_frame(frame)
		, m_outlineStyle(outlineStyle)
		, m_fillStyle(fillStyle)
	{
	}

	void SetFrame(const Frame& frame) override
	{
		m_frame = frame;
	}

	std::optional<Frame> GetFrame() const override
	{
		return m_frame;
	}

	void SetOutlineColor(std::optional<uint32_t> color) override
	{
		m_outlineStyle.SetOutlineColor(color);
	}

	std::optional<uint32_t> GetOutlineColor() const override
	{
		return m_outlineStyle.GetOutlineColor();
	}

	void SetFillColor(std::optional<uint32_t> color) override
	{
		m_fillStyle.SetFillColor(color);
	}

	std::optional<uint32_t> GetFillColor() const override
	{
		return m_fillStyle.GetFillColor();
	}

	void SetOutlineThickness(std::optional<float> thickness) override
	{
		m_outlineStyle.SetThickness(thickness);
	}

	std::optional<float> GetOutlineThickness() const override
	{
		return m_outlineStyle.GetThickness();
	}

private:
	Frame m_frame;
	FillStyle m_fillStyle;
	OutlineStyle m_outlineStyle;
};

class Triangle : public AbstractShape
{
public:
private:
};

class Rectangle : public AbstractShape
{
public:
private:
};

class Ellipse : public AbstractShape
{
public:
private:
};