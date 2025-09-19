#pragma once
#include "Point.h"

Point::Point()
	: m_x{ 0.0 }
	, m_y{ 0.0 }
{
}

Point::Point(const double& ox, const double& oy)
	: m_x{ ox }
	, m_y{ oy }
{
}

double Point::GetX() const 
{
	return m_x;
}
double Point::GetY() const 
{
	return m_y;
}

Point Point::operator+(const Point& other) const 
{
	return Point(m_x + other.m_x, m_y + other.m_y);
}

Point Point::operator-(const Point& other) const 
{
	return Point(m_x - other.m_x, m_y - other.m_y);
}

Point& Point::operator+=(const Point& other) 
{
	m_x += other.m_x;
	m_y += other.m_y;
	return *this;
}
