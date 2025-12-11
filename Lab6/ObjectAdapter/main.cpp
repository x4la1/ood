#include "ModernClassAdapter.h"
#include "ModernGraphicsLib.h"
#include "ModernObjectAdapter.h"
#include "ShapeDrawingLib.h"
#include <iostream>
#include <string>

namespace app
{
void PaintPicture(shape_drawing_lib::CanvasPainter& painter)
{
	using namespace shape_drawing_lib;
	Triangle triangle({ 10, 15 }, { 100, 200 }, { 150, 250 }, 0x112233);
	Rectangle rectangle({ 30, 40 }, 18, 24, 0xFFAAFF);
	painter.Draw(triangle);
	painter.Draw(rectangle);
}

void PaintPictureOnCanvas()
{
	graphics_lib::Canvas simpleCanvas;
	shape_drawing_lib::CanvasPainter painter(simpleCanvas);
	PaintPicture(painter);
}

void PaintPictureOnModernGraphicsRenderer()
{
	// modern_graphics_lib::ModernGraphicsRenderer renderer(std::cout);
	// ModernObjectAdapter adapter(renderer);
	ModernClassAdapter adapter(std::cout);
	shape_drawing_lib::CanvasPainter painter(adapter);
	PaintPicture(painter);
}
} // namespace app

int main()
{
	std::cout << "Should we use new API (y)?\n";
	std::string userInput;
	if (std::getline(std::cin, userInput) && (userInput == "y" || userInput == "Y"))
	{
		app::PaintPictureOnModernGraphicsRenderer();
	}
	else
	{
		app::PaintPictureOnCanvas();
	}
	return 0;
}
