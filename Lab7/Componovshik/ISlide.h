#pragma once
#include "IDrawable.h"
#include "IGroupShapes.h"
#include "IShape.h"

class ISlide : public IDrawable
{
public:
	virtual void AddShape(std::unique_ptr<IShape> shape) = 0;
	virtual void DeleteShapeAtIndex(size_t index) = 0;
	virtual IShape* GetShapeAtIndex(size_t index) = 0;
	virtual void GroupShapesAtIndexes(std::vector<size_t> indexes) = 0;
	virtual size_t GetItemsCount() const = 0;

	virtual ~ISlide() = default;
};

class Slide : public ISlide
{
public:
	void AddShape(std::unique_ptr<IShape> shape) override
	{
		m_shapes.push_back(std::move(shape));
	}

	void DeleteShapeAtIndex(size_t index) override
	{
		RangeCheck(index);
		m_shapes.erase(m_shapes.begin() + index);
	}

	IShape* GetShapeAtIndex(size_t index) override
	{
		RangeCheck(index);
		return m_shapes[index].get();
	}

	void GroupShapesAtIndexes(std::vector<size_t> indexes) override
	{
		for (auto index : indexes)
		{
			RangeCheck(index);
		}

		std::sort(indexes.begin(), indexes.end(), std::greater<size_t>());
		std::vector<std::unique_ptr<IShape>> groupShapes;
		for (auto index : indexes)
		{
			groupShapes.push_back(std::move(m_shapes[index]));
			m_shapes.erase(m_shapes.begin() + index);
		}

		std::reverse(groupShapes.begin(), groupShapes.end());

		auto group = std::make_unique<GroupShape>(std::move(groupShapes));
		AddShape(std::move(group));
	}

	void Draw(ICanvas& canvas) override
	{
		for (auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
	}

	size_t GetItemsCount() const override
	{
		return m_shapes.size();
	}

private:
	void RangeCheck(size_t index)
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index out of range");
		}
	}

	std::vector<std::unique_ptr<IShape>> m_shapes;
};