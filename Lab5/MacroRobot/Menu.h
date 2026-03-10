#pragma once

#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <string>

class Menu
{
public:
	using Command = std::function<void()>;

	void AddItem(const std::string& shortcut, const std::string& description, const Command& command)
	{
		if (IsShortcutExist(shortcut))
		{
			std::cout << "Shortcut already exist\n";
			return;
		}

		m_items.emplace_back(shortcut, description, command);
	}

	void AddMacroCommand(const std::string& shortcut, const std::string& description, const std::vector<std::string>& shortcuts)
	{
		std::vector<Command> commands;

		for (const std::string& shortcut : shortcuts)
		{
			auto it = std::find_if(m_items.begin(), m_items.end(),
				[&](const Item& item)
				{
					return item.shortcut == shortcut;
				});

			if (it != m_items.end())
			{
				commands.push_back(it->command);
			}
		}

		Command macroCommand =
			[commands = std::move(commands)]()
			{
				for (auto& command : commands)
				{
					command();
				}
			};

		m_items.emplace_back(shortcut, description, macroCommand);
	}

	void Run()
	{
		ShowInstructions();

		std::string command;
		while ((std::cout << ">")
			&& getline(std::cin, command)
			&& ExecuteCommand(command))
		{
		}
	}

	void ShowInstructions()const
	{
		std::cout << "Commands list:\n";
		for (auto& item : m_items)
		{
			std::cout << "  " << item.shortcut << ": " << item.description << "\n";
		}
	}

	void Exit()
	{
		m_exit = true;
	}

	bool IsShortcutExist(const std::string& shortcut)
	{
		auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
			return item.shortcut == shortcut;
			});

		return it != m_items.end();
	}

private:
	bool ExecuteCommand(const std::string& shortcut)
	{
		m_exit = false;

		auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
			return item.shortcut == shortcut;
			});
		if (it != m_items.end())
		{
			it->command();
		}
		else
		{
			std::cout << "Unknown command\n";
		}

		return !m_exit;
	}

	struct Item
	{
		Item(const std::string& shortcut, const std::string& description, const Command& command)
			: shortcut(shortcut)
			, description(description)
			, command(command)
		{
		}

		std::string shortcut;
		std::string description;
		Command command;
	};

	bool m_exit = false;
	std::vector<Item> m_items;
};