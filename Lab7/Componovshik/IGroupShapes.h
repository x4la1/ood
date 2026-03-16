#pragma once
#include "IShape.h"

class IGroupShapes : public IShape
{
public:
	virtual void AddShape(std::unique_ptr<IShape> shape) = 0;
	virtual void DeleteShapeByIndex(size_t index) = 0;
	virtual IShape* GetShapeByIndex(size_t index) = 0;

	virtual void GroupShapesByIndexes(std::vector<size_t> indexes) = 0;
	virtual size_t GetShapesCount() const = 0;

	virtual ~IGroupShapes() = default;
};

class GroupShape : public IGroupShapes
{
public:
	GroupShape()
	{
	}

	GroupShape(std::vector<std::unique_ptr<IShape>> shapes)
		: m_shapes(std::move(shapes))
	{
	}

	void SetFrame(const Frame& frame) override
	{
		if (m_shapes.empty())
		{
			return;
		}

		std::optional<Frame> oldFrame = GetFrame();
		if (!oldFrame)
		{
			return;
		}

		float widthScale = frame.GetWidth() / oldFrame->GetWidth();
		float heightScale = frame.GetHeight() / oldFrame->GetHeight();

		for (auto& shape : m_shapes)
		{
			std::optional<Frame> shapeFrame = shape->GetFrame();

			if (!shapeFrame)
			{
				continue;
			}

			float topLeftXOffset = shapeFrame->GetTopLeft().x - oldFrame->GetTopLeft().x;
			float scaledTopLeftX = widthScale * topLeftXOffset;
			float topLeftYOffset = shapeFrame->GetTopLeft().y - oldFrame->GetTopLeft().y;
			float scaledTopLeftY = heightScale * topLeftYOffset;

			shape->SetFrame(Frame(
				{ frame.GetTopLeft().x + scaledTopLeftX, frame.GetTopLeft().y + scaledTopLeftY },
				shapeFrame->GetWidth() * widthScale,
				shapeFrame->GetHeight() * heightScale));
		}
	}

	std::optional<Frame> GetFrame() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		auto firstNonEmptyShape = std::find_if(m_shapes.begin(), m_shapes.end(),
			[](const auto& shape) { return shape->GetFrame().has_value(); });

		if (firstNonEmptyShape == m_shapes.end())
		{
			return std::nullopt;
		}

		Frame firstFrame = *(*firstNonEmptyShape)->GetFrame();

		float minLeftTopX = firstFrame.GetTopLeft().x;
		float minLeftTopY = firstFrame.GetTopLeft().y;
		float maxRightBottomX = firstFrame.GetTopLeft().x + firstFrame.GetWidth();
		float maxRightBottomY = firstFrame.GetTopLeft().y + firstFrame.GetHeight();

		for (auto it = std::next(firstNonEmptyShape); it != m_shapes.end(); ++it)
		{
			std::optional<Frame> shapeFrame = (*it)->GetFrame();
			if (!shapeFrame)
			{
				continue;
			}

			minLeftTopX = std::min(minLeftTopX, shapeFrame->GetTopLeft().x);
			minLeftTopY = std::min(minLeftTopY, shapeFrame->GetTopLeft().y);
			maxRightBottomX = std::max(maxRightBottomX, shapeFrame->GetTopLeft().x + shapeFrame->GetWidth());
			maxRightBottomY = std::max(maxRightBottomY, shapeFrame->GetTopLeft().y + shapeFrame->GetHeight());
		}

		return Frame(
			{ minLeftTopX, minLeftTopY },
			maxRightBottomX - minLeftTopX,
			maxRightBottomY - minLeftTopY);
	}

	void SetOutlineColor(std::optional<uint32_t> color) override
	{
		for (auto& shape : m_shapes)
		{
			shape->SetOutlineColor(color);
		}
	}

	std::optional<uint32_t> GetOutlineColor() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<uint32_t> firstColor = m_shapes[0]->GetOutlineColor();

		for (size_t i = 1; i < m_shapes.size(); ++i)
		{
			std::optional<uint32_t> shapeColor = m_shapes[i]->GetOutlineColor();

			if (firstColor != shapeColor)
			{
				return std::nullopt;
			}
		}

		return firstColor;
	}

	void SetFillColor(std::optional<uint32_t> color) override
	{
		for (auto& shape : m_shapes)
		{
			shape->SetFillColor(color);
		}
	}

	std::optional<uint32_t> GetFillColor() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<uint32_t> firstColor = m_shapes[0]->GetFillColor();

		for (size_t i = 1; i < m_shapes.size(); ++i)
		{
			std::optional<uint32_t> shapeColor = m_shapes[i]->GetFillColor();

			if (firstColor != shapeColor)
			{
				return std::nullopt;
			}
		}

		return firstColor;
	}

	void SetOutlineThickness(std::optional<float> thickness) override
	{
		for (auto& shape : m_shapes)
		{
			shape->SetOutlineThickness(thickness);
		}
	}

	std::optional<float> GetOutlineThickness() const override
	{
		if (m_shapes.empty())
		{
			return std::nullopt;
		}

		std::optional<float> firstThickness = m_shapes[0]->GetOutlineThickness();

		for (size_t i = 1; i < m_shapes.size(); ++i)
		{
			std::optional<float> shapeThickness = m_shapes[i]->GetOutlineThickness();

			if (firstThickness != shapeThickness)
			{
				return std::nullopt;
			}
		}

		return firstThickness;
	}

	void AddShape(std::unique_ptr<IShape> shape) override
	{
		m_shapes.push_back(std::move(shape));
	}

	void DeleteShapeByIndex(size_t index) override
	{
		RangeCheck(index);
		m_shapes.erase(m_shapes.begin() + index);
	}

	IShape* GetShapeByIndex(size_t index) override
	{
		RangeCheck(index);
		return m_shapes[index].get();
	}

	void GroupShapesByIndexes(std::vector<size_t> indexes) override
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

	size_t GetShapesCount() const override
	{
		return m_shapes.size();
	}

	std::unique_ptr<IShape> Clone() override
	{
		std::vector<std::unique_ptr<IShape>> groupShapes;

		for (auto& shape : m_shapes)
		{
			groupShapes.push_back(shape->Clone());
		}

		return std::make_unique<GroupShape>(std::move(groupShapes));
	}

	void Draw(ICanvas& canvas) override
	{
		for (auto& shape : m_shapes)
		{
			shape->Draw(canvas);
		}
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