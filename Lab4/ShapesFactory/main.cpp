#include "Client.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
	std::unique_ptr<IShapeFactory> factory = std::make_unique<ShapeFactory>();
	auto painter = std::make_unique<Painter>();
	auto canvas = std::make_unique<Canvas>(window);
	auto designer = std::make_unique<Designer>(move(factory));
	Client client(move(designer), move(painter), move(canvas));

	try
	{
		client.CreateDraft();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		try
		{
			window.clear(sf::Color::White);
			client.Draw();
			window.display();
		}
		catch (std::exception& e)
		{
			std::cout << e.what();
		}
	}
	return 0;
}