#include "CommandLine.h"
#include "CircleStrategy.h"
#include "LineStrategy.h"
#include "RectangleStrategy.h"
#include "TextStrategy.h"
#include "TriangleStrategy.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

const std::unordered_map<std::string, cli::CommandType> cli::CommandLine::s_commandTypes = {
	{ "AddShape", cli::CommandType::AddShape },
	{ "MoveShape", cli::CommandType::MoveShape },
	{ "MovePicture", cli::CommandType::MovePicture },
	{ "DeleteShape", cli::CommandType::DeleteShape },
	{ "ChangeColor", cli::CommandType::ChangeColor },
	{ "ChangeShape", cli::CommandType::ChangeShape },
	{ "DrawShape", cli::CommandType::DrawShape },
	{ "DrawPicture", cli::CommandType::DrawPicture },
	{ "List", cli::CommandType::List },
};

cli::CommandLine::CommandLine(gfx::ICanvas& canvas, shapes::Picture& picture)
	: m_canvas{ canvas }
	, m_picture{ picture }
{
}

void cli::CommandLine::HandleCommand(const std::string& command)
{
	std::istringstream iss(command);
	std::vector<std::string> tokens{};
	std::string token;

	while (iss >> token)
	{
		tokens.push_back(token);
	}

	if (tokens.empty())
	{
		return;
	}

	switch (GetCommandType(tokens[0]))
	{
	case CommandType::AddShape:
		HandleAddShape(tokens);
		break;
	case CommandType::MoveShape:
		HandleMoveShape(tokens);
		break;
	case CommandType::MovePicture:
		HandleMovePicture(tokens);
		break;
	case CommandType::DeleteShape:
		HandleDeleteShape(tokens);
		break;
	case CommandType::List:
		HandleList();
		break;
	case CommandType::ChangeColor:
		HandleChangeColor(tokens);
		break;
	case CommandType::ChangeShape:
		HandleChangeShape(tokens);
		break;
	case CommandType::DrawShape:
		HandleDrawShape(tokens);
		break;
	case CommandType::DrawPicture:
		HandleDrawPicture();
		break;
	default:
		throw std::invalid_argument("Invalid command\n");
	}
}

cli::CommandType cli::CommandLine::GetCommandType(const std::string& command) const
{
	auto it = s_commandTypes.find(command);
	if (it == s_commandTypes.end())
	{
		throw std::invalid_argument("Invalid command\n");
	}

	return it->second;
}

double cli::CommandLine::ParseDouble(const std::string& line) const
{
	try
	{
		return std::stod(line);
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("Invalid values\n");
	}
}

uint32_t cli::CommandLine::ParseColor(const std::string& line) const
{
	std::regex colorRegex(R"(^#([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})([0-9A-Fa-f]{2})$)");
	std::smatch match;
	if (!std::regex_match(line, match, colorRegex))
	{
		throw std::invalid_argument("Invalid command\n");
	}

	uint32_t r = std::stoi(match[1].str(), nullptr, 16);
	uint32_t g = std::stoi(match[2].str(), nullptr, 16);
	uint32_t b = std::stoi(match[3].str(), nullptr, 16);
	return (r << 16) | (g << 8) | b;
}

void cli::CommandLine::HandleAddShape(const std::vector<std::string>& tokens)
{
	if (tokens.size() < 4)
	{
		throw std::invalid_argument("Invalid command\n");
	}

	std::string id = tokens[1];
	uint32_t color = ParseColor(tokens[2]);
	std::string type = tokens[3];

	std::vector<std::string> params = tokens;
	params.erase(params.begin(), params.begin() + 4);

	auto strategy = CreateStrategy(type, params);
	auto shape = std::make_unique<shapes::Shape>(std::move(strategy), gfx::Color(color));
	m_picture.AddShape(std::move(shape), id);
}

void cli::CommandLine::HandleMovePicture(const std::vector<std::string>& tokens)
{
	if (tokens.size() != 3)
	{
		throw std::invalid_argument("Invalid command\n");
	}

	double dx = ParseDouble(tokens[1]);
	double dy = ParseDouble(tokens[2]);
	m_picture.MovePicture(Point(dx, dy));
}

void cli::CommandLine::HandleMoveShape(const std::vector<std::string>& tokens)
{
	if (tokens.size() != 4)
	{
		throw std::invalid_argument("Invalid command\n");
	}

	std::string id = tokens[1];
	double dx = ParseDouble(tokens[2]);
	double dy = ParseDouble(tokens[3]);
	m_picture.MoveShape(id, Point(dx, dy));
}

void cli::CommandLine::HandleDeleteShape(const std::vector<std::string>& tokens)
{
	if (tokens.size() != 2)
	{
		throw std::invalid_argument("Invalid command\n");
	}

	m_picture.DeleteShape(tokens[1]);
}

void cli::CommandLine::HandleChangeColor(const std::vector<std::string>& tokens)
{
	if (tokens.size() != 3)
	{
		throw std::invalid_argument("Invalid command\n");
	}
	std::string id = tokens[1];
	uint32_t color = ParseColor(tokens[2]);

	m_picture.ChangeShapeColor(id, gfx::Color(color));
}

void cli::CommandLine::HandleChangeShape(const std::vector<std::string>& tokens)
{
	if (tokens.size() < 4)
	{
		throw std::invalid_argument("Invalid command\n");
	}

	std::string id = tokens[1];
	std::string type = tokens[2];
	std::vector<std::string> params = tokens;
	params.erase(params.begin(), params.begin() + 3);

	auto strategy = CreateStrategy(type, params);
	m_picture.ChangeShape(id, std::move(strategy));
}

void cli::CommandLine::HandleDrawShape(const std::vector<std::string>& tokens) const
{
	if (tokens.size() != 2)
	{
		throw std::invalid_argument("Invalid command\n");
	}
	std::string id = tokens[1];
	m_picture.DrawShape(id, m_canvas);
}

void cli::CommandLine::HandleDrawPicture() const
{
	m_picture.DrawPicture(m_canvas);
}

void cli::CommandLine::HandleList() const
{
	std::vector<std::string> list = m_picture.List();
	for (const auto& item : list)
	{
		std::cout << item;
	}
}

// фабрика 
std::unique_ptr<shapes::IShapeStrategy> cli::CommandLine::CreateStrategy(const std::string& type, std::vector<std::string>& params) const
{
	if (type == "circle")
	{
		if (params.size() != 3)
		{
			throw std::invalid_argument("Invalid command\n");
		}

		double cx = ParseDouble(params[0]);
		double cy = ParseDouble(params[1]);
		double radius = ParseDouble(params[2]);
		return std::make_unique<shapes::CircleStrategy>(Point(cx, cy), radius);
	}
	else if (type == "rectangle")
	{
		if (params.size() != 4)
		{
			throw std::invalid_argument("Invalid command\n");
		}
		double left = ParseDouble(params[0]);
		double top = ParseDouble(params[1]);
		double width = ParseDouble(params[2]);
		double height = ParseDouble(params[3]);
		return std::make_unique<shapes::RectangleStrategy>(Point(left, top), width, height);
	}
	else if (type == "triangle")
	{
		if (params.size() != 6)
		{
			throw std::invalid_argument("Invalid command\n");
		}
		double x1 = ParseDouble(params[0]);
		double y1 = ParseDouble(params[1]);
		double x2 = ParseDouble(params[2]);
		double y2 = ParseDouble(params[3]);
		double x3 = ParseDouble(params[4]);
		double y3 = ParseDouble(params[5]);
		return std::make_unique<shapes::TriangleStrategy>(Point(x1, y1), Point(x2, y2), Point(x3, y3));
	}
	else if (type == "line")
	{
		if (params.size() != 4)
		{
			throw std::invalid_argument("Invalid command\n");
		}

		double x1 = ParseDouble(params[0]);
		double y1 = ParseDouble(params[1]);
		double x2 = ParseDouble(params[2]);
		double y2 = ParseDouble(params[3]);
		return std::make_unique<shapes::LineStrategy>(Point(x1, y1), Point(x2, y2));
	}
	else if (type == "text")
	{
		if (params.size() < 4)
		{
			throw std::invalid_argument("Invalid command\n");
		}

		double left = ParseDouble(params[0]);
		double top = ParseDouble(params[1]);
		double fontSize = ParseDouble(params[2]);
		std::string text;
		for (size_t i = 3; i < params.size(); ++i)
		{
			text += " " + params[i];
		}

		return std::make_unique<shapes::TextStrategy>(Point(left, top), text, fontSize);
	}
	else
	{
		throw std::invalid_argument("Invalid command\n");
	}
}