#pragma once
#include <optional>
#include <iostream>
#include <map>

enum class WalkDirection
{
	North,
	South,
	East,
	West,
};

class Robot
{
public:
	void TurnOn()
	{
		if (!m_turnedOn)
		{
			m_turnedOn = true;
			std::cout << "It am waiting for your commands\n";
		}
	}

	void TurnOff()
	{
		if (m_turnedOn)
		{
			m_turnedOn = false;
			m_direction.reset();
			std::cout << "It is a pleasure to serve you\n";
		}
	}

	void Walk(WalkDirection direction)
	{
		using namespace std;

		if (m_turnedOn)
		{
			m_direction = direction;
			static const map<WalkDirection, string> directionToString = {
				{ WalkDirection::East, "east" },
				{ WalkDirection::South, "south" },
				{ WalkDirection::West, "west" },
				{ WalkDirection::North, "north" },
			};
			cout << "Walking " << directionToString.at(direction) << endl;
		}
		else
		{
			cout << "The robot should be turned on first" << endl;
		}
	}

	void Stop()
	{

		if (m_turnedOn)
		{
			if (m_direction)
			{
				m_direction.reset();
				std::cout << "Stopped\n";
			}
			else
			{
				std::cout << "I am staying still\n";
			}
		}
		else
		{
			std::cout << "The robot should be turned on first\n";
		}
	}

private:
	bool m_turnedOn = false;
	std::optional<WalkDirection> m_direction;
};