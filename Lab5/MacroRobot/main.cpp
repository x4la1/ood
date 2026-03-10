#include <iostream>
#include "Robot.h"
#include "Menu.h"

void CreateMacroCommand(Menu& menu)
{
	std::string shortcut, description;
	std::vector<std::string> commands;

	std::cout << "Enter command shortcut: ";
	std::getline(std::cin, shortcut);

	if (menu.IsShortcutExist(shortcut))
	{
		std::cout << "Shortcut already exist\n";
		return;
	}

	std::cout << "Enter command description: ";
	std::getline(std::cin, description);

	std::cout << "Write commands\n";

	std::string command;

	while (std::getline(std::cin, command) && command != "end_macro")
	{
		if (menu.IsShortcutExist(command))
		{
			commands.push_back(command);
		}
		else
		{
			std::cout << "Unknown command\n";
		}
	}

	menu.AddMacroCommand(shortcut, description, commands);
}

void AddCommands(Menu& menu, Robot& robot)
{
	menu.AddItem(
		"on",
		"Turns the Robot on",
		[&]() {robot.TurnOn(); }
	);

	menu.AddItem(
		"off",
		"Turns the Robot iff",
		[&]() {robot.TurnOff(); }
	);

	menu.AddItem(
		"north",
		"Makes the Robot walk north",
		[&]() {robot.Walk(WalkDirection::North); }
	);

	menu.AddItem(
		"south",
		"Makes the Robot walk south",
		[&]() {robot.Walk(WalkDirection::South); }
	);

	menu.AddItem(
		"east",
		"Makes the Robot walk east",
		[&]() {robot.Walk(WalkDirection::East); }
	);

	menu.AddItem(
		"west",
		"Makes the Robot walk west",
		[&]() {robot.Walk(WalkDirection::West); }
	);

	menu.AddItem(
		"stop",
		"Stops the Robot",
		[&]() {robot.Stop(); }
	);

	menu.AddItem(
		"help",
		"Commands instruction",
		[&]() {menu.ShowInstructions(); }
	);

	menu.AddItem(
		"exit",
		"Exit",
		[&]() {menu.Exit(); }
	);

	menu.AddItem(
		"begin_macro",
		"Begin create macro command",
		[&]() {CreateMacroCommand(menu); }
	);
}

int main()
{
	Menu menu;
	Robot robot;
	AddCommands(menu, robot);

	menu.Run();

	return 0;
}