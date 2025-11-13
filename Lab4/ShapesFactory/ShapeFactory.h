#pragma once
#include "Ellipse.h"
#include "Rectangle.h"
#include "RegularPolygon.h"
#include "Triangle.h"
#include <algorithm>
#include <cctype>
#include <regex>
#include <sstream>
#include <string>
//клиент сам все распихивает
class IShapeFactory
{
public:
	using ShapePtr = std::unique_ptr<Shape>;
	virtual ShapePtr CreateShape(const std::string& desription) = 0;
};

/*
1.Создание правильного многоугольника:
	regular_polygon <color> <center x> <center y> <vertex count> <radius>

2.Создание эллипса:
	ellipse <color> <center x> <center y> <horizontal radius> <vertical radius>

3.Создание треугольника:
	triangle <color> <x1> <y1> <x2> <y2> <x3> <y3>

4.Создание прямоугольника:
	rectagnle <color> <left-top x> <left-top y> <right-bottom x> <right-bottom y>

	color = green | red | blue | yellow | pink | black
*/

class ShapeFactory : public IShapeFactory
{
public:
	ShapePtr CreateShape(const std::string& desription) override
	{
		std::smatch matches;
		std::string input = desription;

		if (std::regex_match(input, matches, s_regularPolygonRegex))
		{
			return CreateRegularPolygon(matches);
		}
		else if (std::regex_match(input, matches, s_ellipseRegex))
		{
			return CreateEllise(matches);
		}
		else if (std::regex_match(input, matches, s_triangleRegex))
		{
			return CreateTriangle(matches);
		}
		else if (std::regex_match(input, matches, s_rectangleRegex))
		{
			return CreateRectangle(matches);
		}

		throw std::invalid_argument("Unknown shape format: " + desription + '\n');
	}

private:
	Color StringToColor(const std::string& color)
	{
		auto it = s_stringColor.find(color);
		if (it != s_stringColor.end())
		{
			return it->second;
		}

		throw std::invalid_argument("Unknown color: " + color + '\n');
	}

	ShapePtr CreateRegularPolygon(const std::smatch& matches)
	{
		Color color = StringToColor(matches[1]);
		Point center;
		center.x = std::stod(matches[2]);
		center.y = std::stod(matches[3]);
		uint32_t vertexCount = std::stoi(matches[4]);
		double radius = std::stod(matches[5]);

		return std::make_unique<RegularPolygon>(vertexCount, center, radius, color);
	}

	ShapePtr CreateEllise(const std::smatch& matches)
	{
		Color color = StringToColor(matches[1]);
		Point center;
		center.x = std::stod(matches[2]);
		center.y = std::stod(matches[3]);
		double rx = std::stod(matches[4]);
		double ry = std::stod(matches[5]);

		return std::make_unique<Ellipse>(center, rx, ry, color);
	}

	ShapePtr CreateTriangle(const std::smatch& matches)
	{
		Color color = StringToColor(matches[1]);
		Point p1, p2, p3;
		p1.x = std::stod(matches[2]);
		p1.y = std::stod(matches[3]);
		p2.x = std::stod(matches[4]);
		p2.y = std::stod(matches[5]);
		p3.x = std::stod(matches[6]);
		p3.y = std::stod(matches[7]);

		return std::make_unique<Triangle>(p1, p2, p3, color);
	}

	ShapePtr CreateRectangle(const std::smatch& matches)
	{
		Color color = StringToColor(matches[1]);
		Point leftTop, rightBottom;
		leftTop.x = std::stod(matches[2]);
		leftTop.y = std::stod(matches[3]);
		rightBottom.x = std::stod(matches[4]);
		rightBottom.y = std::stod(matches[5]);

		return std::make_unique<Rectangle>(leftTop, rightBottom, color);
	}

	static const std::regex s_regularPolygonRegex;
	static const std::regex s_ellipseRegex;
	static const std::regex s_triangleRegex;
	static const std::regex s_rectangleRegex;
	static const std::map<std::string, Color> s_stringColor;
};

const std::regex ShapeFactory::s_regularPolygonRegex(
	R"(regular_polygon\s+(green|red|blue|yellow|pink|black)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+(\d+)\s+(\d*\.?\d+))");

const std::regex ShapeFactory::s_ellipseRegex(
	R"(ellipse\s+(green|red|blue|yellow|pink|black)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+(\d*\.?\d+)\s+(\d*\.?\d+))");

const std::regex ShapeFactory::s_triangleRegex(
	R"(triangle\s+(green|red|blue|yellow|pink|black)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+))");

const std::regex ShapeFactory::s_rectangleRegex(
	R"(rectangle\s+(green|red|blue|yellow|pink|black)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+))");

const std::map<std::string, Color> ShapeFactory::s_stringColor = {
	{ "green",
		Color::Green },
	{ "red",
		Color::Red },
	{ "blue",
		Color::Blue },
	{ "yellow",
		Color::Yellow },
	{ "pink",
		Color::Pink },
	{ "black",
		Color::Black },
};