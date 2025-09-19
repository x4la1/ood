#include "Picture.h"
#include <stdexcept>
#include <iostream>

void shapes::Picture::AddShape(std::unique_ptr<Shape> shape, const std::string& id)
{
	if (m_shapes.find(id) != m_shapes.end())
	{
		throw std::invalid_argument("ID already exists\n");
	}

	m_shapes[id] = std::move(shape);
	m_keys.push_back(id);
}

void shapes::Picture::DeleteShape(const std::string& id)
{
	if (!m_shapes.erase(id))
	{
		throw std::invalid_argument("Shape not found\n");
	}

	m_keys.erase(std::remove(m_keys.begin(), m_keys.end(), id), m_keys.end());
}

void shapes::Picture::MoveShape(const std::string& id, const Point& vector)
{
	if (m_shapes.find(id) == m_shapes.end())
	{
		throw std::invalid_argument("Shape not found\n");
	}

	m_shapes.at(id)->MoveShape(vector);
}

void shapes::Picture::MovePicture(const Point& vector)
{
	for (auto& pair : m_shapes)
	{
		pair.second->MoveShape(vector);
	}
}

void shapes::Picture::DrawShape(const std::string& id, gfx::ICanvas& canvas) const
{
	if (m_shapes.find(id) == m_shapes.end())
	{
		throw std::invalid_argument("Shape not found\n");
	}

	m_shapes.at(id)->Draw(canvas);
}

void shapes::Picture::DrawPicture(gfx::ICanvas& canvas) const
{
	for (auto& pair : m_shapes)
	{
		pair.second->Draw(canvas);
	}
}

std::vector<std::string> shapes::Picture::List() const
{
	std::vector<std::string> list;
	uint32_t counter = 0;
	std::string info;
	for (const auto key : m_keys)
	{
		counter += 1;
		info += std::to_string(counter) + " ";
		info += m_shapes.at(key)->GetShapeName() + " ";
		info += key + " ";
		info += m_shapes.at(key)->GetColor().ToString() + " ";
		info += m_shapes.at(key)->ParametersToString() + "\n";
		list.push_back(info);
		info.clear();
	}

	return list;
}

void shapes::Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeStrategy> shapeStrategy)
{
	if (m_shapes.find(id) == m_shapes.end())
	{
		throw std::invalid_argument("Shape not found\n");
	}
	m_shapes.at(id)->SetStrategy(std::move(shapeStrategy));
}

void shapes::Picture::ChangeShapeColor(const std::string& id, const gfx::Color& color)
{
	if (m_shapes.find(id) == m_shapes.end())
	{
		throw std::invalid_argument("Shape not found\n");
	}
	m_shapes.at(id)->ChangeColor(color);
}
