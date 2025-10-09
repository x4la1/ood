#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

template <typename T>
class NumericStats
{
public:
	NumericStats(const std::string& name)
		: m_name{ name }
	{
		if (name.empty())
		{
			throw std::invalid_argument("Name must be not epmty\n");
		}
	}

	void Update(const T& value)
	{
		if (m_min > value)
		{
			m_min = value;
		}
		if (m_max < value)
		{
			m_max = value;
		}
		m_acc += value;
		++m_count;

		std::cout << "Max " << m_name << " " << m_max << "\n";
		std::cout << "Min " << m_name << " " << m_min << "\n";
		std::cout << "Average " << m_name << " " << (m_count > 0 ? m_acc / m_count : 0) << "\n";
		std::cout << "----------------\n";
	}

private:
	std::string m_name;
	T m_min = std::numeric_limits<T>::infinity();
	T m_max = -std::numeric_limits<T>::infinity();
	T m_acc = 0;
	uint32_t m_count = 0;
};