#pragma once
#include <deque>
#include "ICommand.h"

class History
{
public:
	void AddAndExecuteCommand(std::unique_ptr<ICommand>&& command)
	{
		command->Execute();

		if (m_nextCommandIndex < m_commands.size())
		{
			m_commands.erase(m_commands.begin() + m_nextCommandIndex, m_commands.end());
		}

		if (!m_commands.empty() && m_commands.back()->TryMerge(*command))
		{
			return;
		}

		m_commands.emplace_back(std::move(command));

		if (m_commands.size() > s_maxSize)
		{
			m_commands.pop_front();
		}
		else
		{
			m_nextCommandIndex++;
		}
	}

	bool CanUndo() const
	{
		return m_nextCommandIndex > 0;
	}

	bool CanRedo() const
	{
		return m_nextCommandIndex < m_commands.size();
	}

	void Undo()
	{
		if (CanUndo())
		{
			m_commands[m_nextCommandIndex - 1]->Unexecute();
			m_nextCommandIndex--;
		}
		else
		{
			throw std::runtime_error("Cant't undo");
		}
	}

	void Redo()
	{
		if (CanRedo())
		{
			m_commands[m_nextCommandIndex]->Execute();
			m_nextCommandIndex++;
		}
		else
		{
			throw std::runtime_error("Can't redo");
		}
	}

private:
	static const size_t s_maxSize = 10;
	size_t m_nextCommandIndex = 0;
	std::deque<std::unique_ptr<ICommand>> m_commands;
};