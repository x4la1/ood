#pragma once
#include "IShapeStrategy.h"
namespace shapes
{

class TextStrategy : public shapes::IShapeStrategy
{
public:
	TextStrategy(const Point leftTop, const std::string& text, const double& fontSize);
	void Draw(gfx::ICanvas& canvas, const gfx::Color& color) override;
	void Move(const Point& vector) override;
	std::string ParametersToString() const override;
	std::string GetShapeName() const override;

private:
	Point m_leftTop;
	std::string m_text;
	double m_fontSize;
};

} // namespace shapes