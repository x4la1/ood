#pragma once
#include "Color.h"
#include "ICanvas.h"
#include <SFML/Graphics.hpp>

namespace gfx
{
class Canvas : public gfx::ICanvas
{
public:
	Canvas(sf::RenderTexture& window);
	void MoveTo(const double& x, const double& y) override;
	void SetColor(const gfx::Color& color) override;
	void LineTo(const double& x, const double& y) override;
	void DrawEllipse(const double& cx, const double& cy, const double& rx, const double& ry) const override;
	void DrawText(const double& left, const double& top, const double& fontSize, const std::string& text) const override;

private:
	sf::RenderTexture& m_window;
	Point m_currentPoint;
	gfx::Color m_color;
	sf::Font m_font;
};
} // namespace gfx