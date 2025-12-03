#pragma once
#include "MultiGumballMachineDynamic.h"

class Menu;

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual ~ICommand() = default;
};

class InsertQuarterCommand : public ICommand
{
public:
	InsertQuarterCommand(multi_with_dynamic_state::MultiGumballMachine& machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine.InsertQuarter();
	}

private:
	multi_with_dynamic_state::MultiGumballMachine& m_machine;
};

class EjectQuartersCommand : public ICommand
{
public:
	EjectQuartersCommand(multi_with_dynamic_state::MultiGumballMachine& machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine.EjectQuarter();
	}

private:
	multi_with_dynamic_state::MultiGumballMachine& m_machine;
};

class TurnCrankCommand : public ICommand
{
public:
	TurnCrankCommand(multi_with_dynamic_state::MultiGumballMachine& machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine.TurnCrank();
	}

private:
	multi_with_dynamic_state::MultiGumballMachine& m_machine;
};

class RefillGumballsCommand : public ICommand
{
public:
	RefillGumballsCommand(multi_with_dynamic_state::MultiGumballMachine& machine, unsigned gumballsCount)
		: m_machine{ machine }
		, m_gumballsCount{ gumballsCount }
	{
	}

	void Execute() override
	{
		m_machine.RefillGumballs(m_gumballsCount);
	}

private:
	unsigned m_gumballsCount;
	multi_with_dynamic_state::MultiGumballMachine& m_machine;
};

class InfoCommand : public ICommand
{
public:
	InfoCommand(multi_with_dynamic_state::MultiGumballMachine& machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		std::cout << m_machine.ToString();
	}

private:
	multi_with_dynamic_state::MultiGumballMachine& m_machine;
};

class ExitMenuCommand : public ICommand
{
public:
	ExitMenuCommand(Menu& menu)
		: m_menu{ menu }
	{
	}

	void Execute() override;

private:
	Menu& m_menu;
};