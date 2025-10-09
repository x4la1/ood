#include "Canvas.h"
#include "CircleStrategy.h"
#include "CommandLine.h"
#include "Picture.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

//вынести из main (мб отдельный поток)

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!", sf::Style::Default);
	window.clear(sf::Color::White);

	sf::RenderTexture permanentCanvas;
	permanentCanvas.create(1000, 1000);
	permanentCanvas.clear(sf::Color::White);
	permanentCanvas.display();

	sf::Sprite canvasSprite(permanentCanvas.getTexture());

	gfx::Canvas canvas(permanentCanvas);
	shapes::Picture picture;

	cli::CommandLine commandLine(canvas, picture);

	window.display();
	permanentCanvas.display();

	std::string command;

	while (window.isOpen())
	{
		std::getline(std::cin, command);
		if (command.empty())
		{
			continue;
		}

		if (command == "exit")
		{
			window.close();
			continue;
		}

		try
		{
			permanentCanvas.draw(canvasSprite);
			commandLine.HandleCommand(command);
			permanentCanvas.display();
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}

		window.clear(sf::Color::White);
		window.draw(canvasSprite);
		window.display();
	}

	return 0;
}
