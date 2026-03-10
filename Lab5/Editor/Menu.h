#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <sstream>



class Menu
{
public:
	using Command = std::function<void(std::istringstream&)>;

	Menu(std::istream& input, std::ostream& output)
		:m_input(input),
		m_output(output)
	{
	}

	void AddItem(const std::string& shortcut, const std::string& description, const Command& command)
	{
		m_items.emplace_back(shortcut, description, command);
	}

	void Run()
	{
		ShowInstructions();

		std::string command;
		while ((m_output << ">") && std::getline(m_input, command) && !m_exit)
		{
			try
			{
				if (ExecuteCommand(command))
				{
					break;
				}
			}
			catch (std::exception& exception)
			{
				m_output << exception.what() << "\n";
			}
		}
	}

	bool ExecuteCommand(const std::string& command)
	{
		std::istringstream iss(command);
		std::string name;
		iss >> name;

		m_exit = false;
		auto it = std::find_if(m_items.begin(), m_items.end(),
			[&](const Item& item)
			{
				return item.shortcut == name;
			});

		if (it != m_items.end())
		{
			it->command(iss);
		}
		else
		{
			throw std::runtime_error("Invalid command");
		}

		return m_exit;
	}

	void ShowInstructions()
	{
		m_output << "Commands list:\n";
		for (auto& item : m_items)
		{
			m_output << item.shortcut << ": " << item.description << "\n";
		}
	}

	void Exit()
	{
		m_exit = true;
	}

private:
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

	std::vector<Item> m_items;
	bool m_exit = false;
	std::istream& m_input;
	std::ostream& m_output;
};