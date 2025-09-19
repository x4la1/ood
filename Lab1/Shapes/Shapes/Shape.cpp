#include "Shape.h"

shapes::Shape::Shape(std::unique_ptr<IShapeStrategy> shapeStrategy, const gfx::Color& color)
	: m_shapeStrategy(std::move(shapeStrategy))
	, m_color{ color }
{
}

void shapes::Shape::SetStrategy(std::unique_ptr<IShapeStrategy> shapeStrategy)
{
	m_shapeStrategy = std::move(shapeStrategy);
}

void shapes::Shape::Draw(gfx::ICanvas& canvas) const
{
	m_shapeStrategy->Draw(canvas, m_color);
}

void shapes::Shape::MoveShape(const Point& vector)
{
	m_shapeStrategy->Move(vector);
}

void shapes::Shape::ChangeColor(const gfx::Color& color)
{
	m_color = color;
}

std::string shapes::Shape::ParametersToString() const
{
	return m_shapeStrategy->ParametersToString();
}

std::string shapes::Shape::GetShapeName() const
{
	return m_shapeStrategy->GetShapeName();
}

gfx::Color shapes::Shape::GetColor() const
{
	return m_color;
}