#pragma once
#include "Command.h"
#include <ranges>
#include <string>
#include <vector>

class Menu
{
public:
	void AddItem(const std::string& shortcut, std::unique_ptr<ICommand>&& command)
	{
		m_items.emplace_back(shortcut, move(command));
	}

	void Run()
	{
		std::string command;
		while ((std::cout << ">") && std::getline(std::cin, command) && ExecuteCommand(command))
		{
		}
	}

	void Exit()
	{
		m_exit = true;
	}

private:
	struct Item
	{
		Item(const std::string& shortcut, std::unique_ptr<ICommand>&& command)
			: m_shortcut{ shortcut }
			, m_command{ move(command) }
		{
		}

		std::string m_shortcut;
		std::unique_ptr<ICommand> m_command;
	};

	bool ExecuteCommand(const std::string& command)
	{
		m_exit = false;
		auto it = std::ranges::find_if(m_items, [&](const Item& item) {
			return item.m_shortcut == command;
		});

		if (it != m_items.end())
		{
			it->m_command->Execute();
		}
		else
		{
			std::cout << "Unknown command\n";
		}

		return !m_exit;
	}

	std::vector<Item> m_items;
	bool m_exit = false;
};

inline void ExitMenuCommand::Execute()
{
	m_menu.Exit();
}