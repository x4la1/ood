#pragma once
#include "Menu.h"
#include "MultiGumballMachineDynamic.h"
#include <iostream>

using namespace multi_with_dynamic_state;

int main()
{
	MultiGumballMachine machine(0);
	Menu menu;

	menu.AddItem("insert", std::make_unique<InsertQuarterCommand>(machine));
	menu.AddItem("eject", std::make_unique<EjectQuartersCommand>(machine));
	menu.AddItem("turn", std::make_unique<TurnCrankCommand>(machine));
	menu.AddItem("refill", std::make_unique<RefillGumballsCommand>(machine, 5));
	menu.AddItem("exit", std::make_unique<ExitMenuCommand>(menu));
	menu.AddItem("info", std::make_unique<InfoCommand>(machine));
	menu.Run();

	return 0;
}
