#pragma once
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numbers>

class WindDirectionStats
{
public:
	void Update(const double& angle)
	{
		double radian = angle * std::numbers::pi / 180.0;
		m_sinSum += std::sin(radian);
		m_cosSum += std::cos(radian);
		m_countAcc++;

		double avgAngle = std::atan2(m_sinSum / m_countAcc, m_cosSum / m_countAcc) * 180.0 / std::numbers::pi;
		if (avgAngle < 0)
		{
			avgAngle += 360.0;
		}

		std::cout << "Average Wind direciton " << std::fixed << std::setprecision(0) << avgAngle << "\n";
		std::cout << "----------------\n";
	};

private:
	double m_sinSum = 0;
	double m_cosSum = 0;
	unsigned m_countAcc = 0;
};