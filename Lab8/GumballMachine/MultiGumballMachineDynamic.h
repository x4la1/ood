#pragma once

#include <format>
#include <iostream>

namespace multi_with_dynamic_state
{

struct IState
{
	virtual void InsertQuarter() = 0;
	virtual void EjectQuarter() = 0;
	virtual void TurnCrank() = 0;
	virtual void Dispense() = 0;
	virtual void Refill(unsigned count) = 0;

	virtual std::string ToString() const = 0;
	virtual ~IState() = default;
};

struct IGumballMachine
{
	virtual void ReleaseBall() = 0;
	virtual unsigned GetBallCount() const = 0;

	virtual void AddQuarter() = 0;
	virtual unsigned GetQuartersCount() const = 0;
	virtual void EjectQuarters() = 0;

	virtual void Refill(unsigned count) = 0;

	virtual void SetSoldOutState() = 0;
	virtual void SetNoQuarterState() = 0;
	virtual void SetSoldState() = 0;
	virtual void SetHasQuarterState() = 0;

	virtual ~IGumballMachine() = default;
};

class SoldState : public IState
{
public:
	SoldState(IGumballMachine& gumballMachine)
		: m_gumballMachine(gumballMachine)
	{
	}

	void InsertQuarter() override
	{
		std::cout << "Please wait, we're already giving you a gumball\n";
	}

	void EjectQuarter() override
	{
		std::cout << "Sorry you already turned the crank\n";
	}

	void TurnCrank() override
	{
		std::cout << "Turning twice doesn't get you another gumball\n";
	}

	void Dispense() override
	{
		m_gumballMachine.ReleaseBall();
		if (m_gumballMachine.GetBallCount() == 0)
		{
			std::cout << "Oops, out of gumballs\n";
			m_gumballMachine.SetSoldOutState();
		}
		else if (m_gumballMachine.GetQuartersCount() == 0)
		{
			m_gumballMachine.SetNoQuarterState();
		}
		else
		{
			m_gumballMachine.SetHasQuarterState();
		}
	}

	void Refill(unsigned count) override
	{
		std::cout << "Please wait\n";
	}

	std::string ToString() const override
	{
		return "delivering a gumball";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class SoldOutState : public IState
{
public:
	SoldOutState(IGumballMachine& gumballMachine)
		: m_gumballMachine(gumballMachine)
	{
	}

	void InsertQuarter() override
	{
		std::cout << "You can't insert a quarter, the machine is sold out\n";
	}

	void EjectQuarter() override
	{
		if (m_gumballMachine.GetQuartersCount() == 0)
		{
			std::cout << "There is no quarters\n";
		}
		else
		{
			m_gumballMachine.EjectQuarters();
			std::cout << "Quarter returned\n";
		}
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no gumballs\n";
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	void Refill(unsigned count) override
	{
		m_gumballMachine.Refill(count);
		if (m_gumballMachine.GetBallCount() == 0)
		{
			return;
		}

		if (m_gumballMachine.GetQuartersCount() == 0)
		{
			m_gumballMachine.SetNoQuarterState();
		}
		else
		{
			m_gumballMachine.SetHasQuarterState();
		}
		std::cout << "Gumballs refilled\n";
	}

	std::string ToString() const override
	{
		return "sold out";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class HasQuarterState : public IState
{
public:
	HasQuarterState(IGumballMachine& gumballMachine)
		: m_gumballMachine(gumballMachine)
	{
	}

	void InsertQuarter() override
	{
		if (m_gumballMachine.GetQuartersCount() != 5)
		{
			m_gumballMachine.AddQuarter();
			std::cout << "You inserted a quarter\n";
		}
		else
		{
			std::cout << "You can't insert more than 5 quarters\n";
		}
	}

	void EjectQuarter() override
	{
		std::cout << "Quarter returned\n";
		m_gumballMachine.EjectQuarters();
		m_gumballMachine.SetNoQuarterState();
	}

	void TurnCrank() override
	{
		std::cout << "You turned...\n";
		m_gumballMachine.SetSoldState();
	}

	void Dispense() override
	{
		std::cout << "No gumball dispensed\n";
	}

	void Refill(unsigned count) override
	{
		m_gumballMachine.Refill(count);
		if (m_gumballMachine.GetBallCount() == 0)
		{
			m_gumballMachine.SetSoldOutState();
		}

		std::cout << "Gumballs refilled\n";
	}

	std::string ToString() const override
	{
		return "waiting for turn of crank";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class NoQuarterState : public IState
{
public:
	NoQuarterState(IGumballMachine& gumballMachine)
		: m_gumballMachine(gumballMachine)
	{
	}

	void InsertQuarter() override
	{
		std::cout << "You inserted a quarter\n";
		m_gumballMachine.AddQuarter();
		m_gumballMachine.SetHasQuarterState();
	}

	void EjectQuarter() override
	{
		std::cout << "You haven't inserted a quarter\n";
	}

	void TurnCrank() override
	{
		std::cout << "You turned but there's no quarter\n";
	}

	void Dispense() override
	{
		std::cout << "You need to pay first\n";
	}

	void Refill(unsigned count) override
	{
		m_gumballMachine.Refill(count);
		if (m_gumballMachine.GetBallCount() == 0)
		{
			m_gumballMachine.SetSoldOutState();
		}
		std::cout << "Gumballs refilled\n";
	}

	std::string ToString() const override
	{
		return "waiting for quarter";
	}

private:
	IGumballMachine& m_gumballMachine;
};

class MultiGumballMachine : private IGumballMachine
{
public:
	MultiGumballMachine(unsigned numBalls)
		: m_gumballsCount(numBalls)
	{

		if (m_gumballsCount > 0)
		{
			SetNoQuarterState();
		}
		else
		{
			SetSoldOutState();
		}
	}

	void EjectQuarter()
	{
		m_currentState->EjectQuarter();
	}

	void InsertQuarter()
	{
		m_currentState->InsertQuarter();
	}

	void TurnCrank()
	{
		m_currentState->TurnCrank();
		m_currentState->Dispense();
	}

	void RefillGumballs(unsigned count)
	{
		m_currentState->Refill(count);
	}

	std::string ToString() const
	{
		return std::format(R"(
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016
Inventory: {} gumball{} {} quarter{}
Machine is {}
)",
			m_gumballsCount, m_gumballsCount != 1 ? "s" : "", m_quartersCount, m_quartersCount != 1 ? "s" : "",
			m_currentState->ToString());
	}

private:
	unsigned GetBallCount() const override
	{
		return m_gumballsCount;
	}

	virtual void ReleaseBall() override
	{
		if (m_gumballsCount != 0 && m_quartersCount != 0)
		{
			std::cout << "A gumball comes rolling out the slot...\n";
			--m_gumballsCount;
			--m_quartersCount;
		}
	}

	unsigned GetQuartersCount() const override
	{
		return m_quartersCount;
	}

	void AddQuarter() override
	{
		if (m_quartersCount < 5)
		{
			++m_quartersCount;
		}
	}

	void EjectQuarters() override
	{
		m_quartersCount = 0;
	}

	void Refill(unsigned count) override
	{
		if (count <= 10)
		{
			m_gumballsCount = count;
		}
	}

	void SetSoldOutState() override
	{
		m_currentState.reset(new SoldOutState(*this));
	}

	void SetNoQuarterState() override
	{
		m_currentState.reset(new NoQuarterState(*this));
	}

	void SetSoldState() override
	{
		m_currentState.reset(new SoldState(*this));
	}

	void SetHasQuarterState() override
	{
		m_currentState.reset(new HasQuarterState(*this));
	}

private:
	unsigned m_gumballsCount = 0;
	unsigned m_quartersCount = 0;
	std::unique_ptr<IState> m_currentState;
};
} // namespace multi_with_dynamic_state