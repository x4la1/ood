#pragma once
#include "Shape.h"
#include <unordered_map>

namespace shapes
{
class Picture
{
public:
	Picture() = default;
	void AddShape(std::unique_ptr<Shape> shape, const std::string& id);
	void DeleteShape(const std::string& id);
	void MoveShape(const std::string& id, const Point& vector);
	void MovePicture(const Point& vector);
	void DrawShape(const std::string& id, gfx::ICanvas& canvas) const;
	void DrawPicture(gfx::ICanvas& canvas) const;
	void ChangeShape(const std::string& id, std::unique_ptr<IShapeStrategy> shapeStrategy);
	void ChangeShapeColor(const std::string& id, const gfx::Color& color);
	std::vector<std::string> List() const;

private:
	std::unordered_map<std::string, std::unique_ptr<Shape>> m_shapes;
	std::vector<std::string> m_keys;
};

} // namespace shapes