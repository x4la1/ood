#pragma once
#include "Shape.h"

class PictureDraft
{
public:
	using ShapePtr = std::unique_ptr<Shape>;

	void AddShape(std::unique_ptr<Shape>&& shape)
	{
		m_shapes.push_back(move(shape));
	}

	size_t GetShapeCount() const { return m_shapes.size(); }

	Shape& GetShape(size_t index) const
	{
		if (index >= m_shapes.size())
		{
			throw std::out_of_range("Index out of range\n");
		}

		return *m_shapes[index];
	}

private:
	std::vector<ShapePtr> m_shapes{};
};
