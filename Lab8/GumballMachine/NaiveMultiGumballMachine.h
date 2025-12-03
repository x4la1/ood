#pragma once

#pragma once

#include <format>
#include <iostream>

namespace multi_naive
{
class MultiGumballMachine
{
public:
	enum class State
	{
		SoldOut, // Жвачка закончилась
		NoQuarter, // Нет монетки
		HasQuarter, // Есть монетка
		Sold, // Жвачка выдана
	};

	MultiGumballMachine(unsigned count)
		: m_gumballsCount(count)
		, m_state(count > 0 ? State::NoQuarter : State::SoldOut)
		, m_quartersCount(0)
	{
	}

	void InsertQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			cout << "You can't insert a quarter, the machine is sold out\n";
			break;
		case State::NoQuarter:
			cout << "You inserted a quarter\n";
			AddQuarter();
			m_state = State::HasQuarter;
			break;
		case State::HasQuarter:
			AddQuarter();
			cout << "You inserted a quarter\n";
			break;
		case State::Sold:
			cout << "Please wait, we're already giving you a gumball\n";
			break;
		}
	}

	void EjectQuarter()
	{
		using namespace std;
		switch (m_state)
		{
		case State::HasQuarter:
			cout << "Quarter returned\n";
			m_quartersCount = 0;
			m_state = State::NoQuarter;
			break;
		case State::NoQuarter:
			cout << "You haven't inserted a quarter\n";
			break;
		case State::Sold:
			cout << "Sorry you already turned the crank\n";
			break;
		case State::SoldOut:
			m_quartersCount = 0;
			cout << "Quarter returned\n";
			break;
		}
	}

	void TurnCrank()
	{
		using namespace std;
		switch (m_state)
		{
		case State::SoldOut:
			cout << "You turned but there's no gumballs\n";
			break;
		case State::NoQuarter:
			cout << "You turned but there's no quarter\n";
			break;
		case State::HasQuarter:
			cout << "You turned...\n";
			m_state = State::Sold;
			--m_quartersCount;
			Dispense();
			break;
		case State::Sold:
			cout << "Turning twice doesn't get you another gumball\n";
			break;
		}
	}

	void Refill(unsigned count)
	{
		if (m_state == State::Sold)
		{
			std::cout << "You can't refill now\n";
			return;
		}

		m_gumballsCount = count;
		if (m_gumballsCount == 0)
		{
			m_state = State::SoldOut;
		}
		else
		{
			if (m_quartersCount == 0)
			{
				m_state = State::NoQuarter;
			}
			else
			{
				m_state = State::HasQuarter;
			}
		}
	}

	std::string ToString() const
	{
		std::string state = (m_state == State::SoldOut)
			? "sold out"
			: (m_state == State::NoQuarter)	 ? "waiting for quarter"
			: (m_state == State::HasQuarter) ? "waiting for turn of crank"
											 : "delivering a gumball";

		return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: {} gumball{} {} quarter{}
Machine is {}
						)",
			m_gumballsCount, m_gumballsCount != 1 ? "s" : "", m_quartersCount, m_quartersCount != 1 ? "s" : "", state);
	}

private:
	void AddQuarter()
	{
		if (m_quartersCount != 5)
		{
			++m_quartersCount;
		}
	}

	void Dispense()
	{
		using namespace std;
		switch (m_state)
		{
		case State::Sold:
			cout << "A gumball comes rolling out the slot\n";
			--m_gumballsCount;
			if (m_gumballsCount == 0)
			{
				cout << "Oops, out of gumballs\n";
				m_state = State::SoldOut;
			}
			else
			{
				if (m_quartersCount == 0)
				{
					m_state = State::NoQuarter;
				}
				else
				{
					m_state = State::HasQuarter;
				}
			}
			break;
		case State::NoQuarter:
			cout << "You need to pay first\n";
			break;
		case State::SoldOut:
		case State::HasQuarter:
			cout << "No gumball dispensed\n";
			break;
		}
	}

	unsigned m_gumballsCount;
	unsigned m_quartersCount;
	State m_state = State::SoldOut;
};
} // namespace multi_naive