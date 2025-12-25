#pragma once
#include "CommonTypes.h"
#include "ICanvas.h"
#include "IShapeStyle.h"
#include <functional>
#include <memory>
#include <optional>

class IGroup;

class IShape
{
public:
	virtual RectD GetFrame() const = 0;
	virtual void SetFrame(const RectD& frame) = 0;
	virtual IShapeStyle& GetShapeStyle() = 0;
	virtual void Draw(ICanvas& canvas) = 0;
	virtual std::shared_ptr<IGroup> GetGroup() = 0;

	~IShape() = default;
};

class IGroup : public IShape
{
	virtual uint32_t GetShapesCount() const = 0;
	virtual std::shared_ptr<IShape> GetShapeAtIndex(uint32_t index) const = 0;
	virtual void InsertShape(std::shared_ptr<IShape>, uint32_t index) = 0;
	virtual void RemoveShapeAtIndex(uint32_t index) = 0;
};

class Shape : public IShape
{
public:
	Shape(const RectD& frame)
	{
		if (frame.m_height < 0 || frame.m_width < 0)
		{
			throw std::invalid_argument("Width and height must be positive");
		}
	}

	RectD GetFrame() const override
	{
		return m_frame;
	}

	void SetFrame(const RectD& frame) override
	{
		if (frame.m_height < 0 || frame.m_width < 0)
		{
			throw std::invalid_argument("Width and height must be positive");
		}

		m_frame = frame;
	}

	IShapeStyle& GetShapeStyle() override
	{
		return m_style;
	}

	std::shared_ptr<IGroup> GetGroup()
	{
		return nullptr;
	}

protected:
	RectD m_frame = { 0, 0, 0, 0 };

private:
	SimpleShapeStyle m_style;
};

class Ellipse : public Shape
{
public:
	Ellipse(const RectD& frame)
		: Shape(frame)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		// рисуем
	}
};

class Rectangle : public Shape
{
public:
	Rectangle(const RectD& frame)
		: Shape(frame)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		// рисуем
	}
};

class Triangle : public Shape
{
public:
	Triangle(const RectD& frame)
		: Shape(frame)
	{
	}

	void Draw(ICanvas& canvas) override
	{
		// рисуем
	}
};