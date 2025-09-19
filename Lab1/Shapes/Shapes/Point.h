#pragma once
#include <cstdint>

class Point
{
public:
	Point();
	Point(const double& ox, const double& oy);
	double GetX() const;
	double GetY() const;

	Point& operator=(const Point& other) = default;
	Point operator+(const Point& other) const;
	Point operator-(const Point& other) const;
	Point& operator+=(const Point& other);

private:
	double m_x;
	double m_y;
};