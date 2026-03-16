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
	AbstractShape(const Frame& frame, const OutlineStyle& outlineStyle, const FillStyle& fillStyle)
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

	void Draw(ICanvas& canvas) override
	{
		auto outlineColor = m_outlineStyle.GetOutlineColor();
		canvas.SetOutlineColor(outlineColor.value_or(0x00000000));

		auto fillColor = m_fillStyle.GetFillColor();
		canvas.SetFillColor(fillColor.value_or(0x00000000));

		auto outlineThickness = m_outlineStyle.GetThickness();
		canvas.SetOutlineThickness(outlineThickness.value_or(0.f));

		OnDraw(canvas);
	}

protected:
	virtual void OnDraw(ICanvas& canvas) = 0;

	Frame m_frame;
	FillStyle m_fillStyle;
	OutlineStyle m_outlineStyle;
};

class Triangle : public AbstractShape
{
public:
	Triangle(const Frame& frame, const OutlineStyle& outlineStyle, const FillStyle& fillStyle)
		: AbstractShape(frame, outlineStyle, fillStyle)
	{
	}

	std::unique_ptr<IShape> Clone() override
	{
		return std::make_unique<Triangle>(m_frame, m_outlineStyle, m_fillStyle);
	}

	void OnDraw(ICanvas& canvas) override
	{
		Point p1, p2, p3;
		p1 = {
			m_frame.GetTopLeft().x,
			m_frame.GetTopLeft().y + m_frame.GetHeight()
		};
		p2 = {
			m_frame.GetTopLeft().x + m_frame.GetWidth(),
			m_frame.GetTopLeft().y + m_frame.GetHeight()
		};
		p3 = {
			m_frame.GetTopLeft().x + m_frame.GetWidth() / 2.f,
			m_frame.GetTopLeft().y
		};

		std::vector<Point> points = { p1, p2, p3 };

		canvas.FillPolygon(points);
		canvas.DrawLine(p1, p2);
		canvas.DrawLine(p2, p3);
		canvas.DrawLine(p3, p1);
	}
};

class Rectangle : public AbstractShape
{
public:
	Rectangle(const Frame& frame, const OutlineStyle& outlineStyle, const FillStyle& fillStyle)
		: AbstractShape(frame, outlineStyle, fillStyle)
	{
	}

	std::unique_ptr<IShape> Clone() override
	{
		return std::make_unique<Rectangle>(m_frame, m_outlineStyle, m_fillStyle);
	}

	void OnDraw(ICanvas& canvas) override
	{
		Point p1, p2, p3, p4;
		p1 = {
			m_frame.GetTopLeft().x,
			m_frame.GetTopLeft().y
		};

		p2 = {
			m_frame.GetTopLeft().x + m_frame.GetWidth(),
			m_frame.GetTopLeft().y
		};

		p3 = {
			m_frame.GetTopLeft().x + m_frame.GetWidth(),
			m_frame.GetTopLeft().y + m_frame.GetHeight()
		};

		p4 = {
			m_frame.GetTopLeft().x,
			m_frame.GetTopLeft().y + m_frame.GetHeight()
		};

		std::vector<Point> points = { p1, p2, p3, p4 };

		canvas.FillPolygon(points);
		canvas.DrawLine(p1, p2);
		canvas.DrawLine(p2, p3);
		canvas.DrawLine(p3, p4);
		canvas.DrawLine(p4, p1);
	}
};

class Ellipse : public AbstractShape
{
public:
	Ellipse(const Frame& frame, const OutlineStyle& outlineStyle, const FillStyle& fillStyle)
		: AbstractShape(frame, outlineStyle, fillStyle)
	{
	}

	std::unique_ptr<IShape> Clone() override
	{
		return std::make_unique<Ellipse>(m_frame, m_outlineStyle, m_fillStyle);
	}

	void OnDraw(ICanvas& canvas) override
	{
		canvas.FillEllipse(
			{ m_frame.GetTopLeft().x + m_frame.GetWidth() / 2.f, m_frame.GetTopLeft().y + m_frame.GetHeight() / 2 },
			m_frame.GetWidth(),
			m_frame.GetHeight());

		canvas.DrawEllipse(
			{ m_frame.GetTopLeft().x + m_frame.GetWidth() / 2.f, m_frame.GetTopLeft().y + m_frame.GetHeight() / 2 },
			m_frame.GetWidth(),
			m_frame.GetHeight());
	}
};