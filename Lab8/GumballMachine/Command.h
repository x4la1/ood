#pragma once
#include "MultiGumballMachineDynamic.h"

using MachinePtr = std::shared_ptr<multi_with_dynamic_state::MultiGumballMachine>;

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
	InsertQuarterCommand(MachinePtr machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine->InsertQuarter();
	}

private:
	MachinePtr m_machine;
};

class EjectQuartersCommand : public ICommand
{
public:
	EjectQuartersCommand(MachinePtr& machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine->EjectQuarter();
	}

private:
	MachinePtr m_machine;
};

class TurnCrankCommand : public ICommand
{
public:
	TurnCrankCommand(MachinePtr machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		m_machine->TurnCrank();
	}

private:
	MachinePtr m_machine;
};

class RefillGumballsCommand : public ICommand
{
public:
	RefillGumballsCommand(MachinePtr machine, unsigned gumballsCount)
		: m_machine{ machine }
		, m_gumballsCount{ gumballsCount }
	{
	}

	void Execute() override
	{
		m_machine->RefillGumballs(m_gumballsCount);
	}

private:
	unsigned m_gumballsCount;
	MachinePtr m_machine;
};

class InfoCommand : public ICommand
{
public:
	InfoCommand(MachinePtr machine)
		: m_machine{ machine }
	{
	}

	void Execute() override
	{
		std::cout << m_machine->ToString();
	}

private:
	MachinePtr m_machine;
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