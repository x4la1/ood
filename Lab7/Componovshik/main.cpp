#include "ISlide.h"
#include <SFML/Graphics.hpp>
#include <iostream>

std::unique_ptr<ISlide> GetSlide()
{
	auto slide = std::make_unique<Slide>();
	auto wall = std::make_unique<Rectangle>(Frame({ 240, 210 }, 450, 275), OutlineStyle(std::nullopt, std::nullopt), FillStyle(0xb86e00ff));
	auto roof = std::make_unique<Triangle>(Frame({ 240, 40 }, 450, 170), OutlineStyle(0x000000FF, 1.f), FillStyle(0xffab2eff));
	auto chimney = std::make_unique<Rectangle>(Frame({ 550, 90 }, 40, 80), OutlineStyle(0x000000FF, std::nullopt), FillStyle(0x0008ffff));
	auto grass = std::make_unique<Rectangle>(Frame({ 40, 430 }, 850, 180), OutlineStyle(0x00000000, 1.f), FillStyle(0x04ff00ff));
	auto window = std::make_unique<Rectangle>(Frame({ 320, 280 }, 100, 100), OutlineStyle(0x000000FF, 1.f), FillStyle(0xffffffff));
	auto door = std::make_unique<Rectangle>(Frame({ 525, 275 }, 100, 190), OutlineStyle(0x000000FF, 2.f), FillStyle(0xb86e00ff));

	std::vector<std::unique_ptr<IShape>> house, fullRoof;
	fullRoof.push_back(std::move(roof));
	fullRoof.push_back(std::move(chimney));

	house.push_back(std::move(wall));
	house.push_back(std::move(window));
	house.push_back(std::move(door));

	auto fullRoofGroupShape = std::make_unique<GroupShape>(std::move(fullRoof));
	auto houseGroupShape = std::make_unique<GroupShape>(std::move(house));

	houseGroupShape->AddShape(std::move(fullRoofGroupShape));

	//houseGroupShape->SetFrame(Frame({ 0, 0 }, 200, 200));

	slide->AddShape(std::move(grass));
	slide->AddShape(std::move(houseGroupShape));

	auto frame = slide->GetShapeAtIndex(0)->GetFrame();

	return slide;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	SFMLCanvas canvas(window);
	auto slide = GetSlide();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		slide->Draw(canvas);
		window.display();
	}

	return 0;
}