#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
class Beverage : public IBeverage
{
public:
	Beverage(const std::string& description)
		: m_description(description)
	{
	}

	std::string GetDescription() const override
	{
		return m_description;
	}

private:
	std::string m_description;
};

enum class CoffePortion
{
	Standart,
	Double,
};

// Кофе
class Coffee : public Beverage
{
public:
	Coffee(CoffePortion coffePortion = CoffePortion::Standart, const std::string& description = "Coffee")
		: m_coffePortion(coffePortion)
		, Beverage(description)
	{
	}

	double GetCost() const override
	{
		return 60;
	}

protected:
	CoffePortion m_coffePortion;
};

// Капуччино
class Cappuccino : public Coffee
{
public:
	Cappuccino(CoffePortion coffePortion = CoffePortion::Standart)
		: Coffee(coffePortion, "Cappuccino")
	{
	}

	std::string GetDescription() const override
	{
		return (m_coffePortion == CoffePortion::Standart
				? "Standart Cappuccino"
				: "Double Cappuccino");
	}

	double GetCost() const override
	{
		return (m_coffePortion == CoffePortion::Standart ? 80 : 120);
	}
};

// Латте
class Latte : public Coffee
{
public:
	Latte(CoffePortion coffePortion = CoffePortion::Standart)
		: Coffee(coffePortion, "Latte")
	{
	}

	std::string GetDescription() const override
	{
		return (m_coffePortion == CoffePortion::Standart
				? "Standart Latte"
				: "Double Latte");
	}

	double GetCost() const override
	{
		return (m_coffePortion == CoffePortion::Standart ? 90 : 130);
	}
};

enum class TeaSort
{
	Black,
	Green,
	Ulun,
	White
};

// Чай
class Tea : public Beverage
{
public:
	Tea(TeaSort teaSort = TeaSort::Black)
		: Beverage("Tea")
		, m_teaSort(teaSort)
	{
	}

	std::string GetDescription() const override
	{
		switch (m_teaSort)
		{
		case TeaSort::Black:
			return "Black tea";
		case TeaSort::Green:
			return "Green tea";
		case TeaSort::Ulun:
			return "Ulun tea";
		case TeaSort::White:
			return "White tea";
		default:
			break;
		}
	}

	double GetCost() const override
	{
		return 30;
	}

private:
	TeaSort m_teaSort;
};

enum class MilkshakePortion
{
	Small,
	Medium,
	Large
};

// Молочный коктейль
class Milkshake : public Beverage
{
public:
	Milkshake(MilkshakePortion milkshakePortion = MilkshakePortion::Medium)
		: Beverage("Milkshake")
		, m_milkshakePortion(milkshakePortion)
	{
	}

	double GetCost() const override
	{
		switch (m_milkshakePortion)
		{
		case MilkshakePortion::Small:
			return 50;
		case MilkshakePortion::Medium:
			return 60;
		case MilkshakePortion::Large:
			return 80;
		default:
			break;
		}
	}

private:
	MilkshakePortion m_milkshakePortion;
};