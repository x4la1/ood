#pragma once
#include "Command.h"
#include <vector>

class IHistory
{
public:
	virtual bool CanUndo() const = 0;
	virtual bool CanRedo() const = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual void AddAndExecuteCommand(const std::shared_ptr<ICommand>& command) = 0;
	virtual ~IHistory() = default;
};

class History : public IHistory
{
public:
	History()
	{
	}

	bool CanUndo() const override
	{
		return m_currentCommandIndex > 0;
	}

	bool CanRedo() const override
	{
		return m_currentCommandIndex < m_commands.size();
	}

	void Undo() override
	{
		if (!CanUndo())
		{
			throw std::runtime_error("Cannot undo");
		}

		--m_currentCommandIndex;
		m_commands[m_currentCommandIndex]->UnExecute();
	}

	void Redo() override
	{
		if (!CanRedo())
		{
			throw std::runtime_error("Cannot redo");
		}

		++m_currentCommandIndex;
		m_commands[m_currentCommandIndex]->Execute();
	}

	void AddAndExecuteCommand(const std::shared_ptr<ICommand>& command) override
	{
		//TODO
	}

private:
	std::vector<std::shared_ptr<UndoableCommand>> m_commands;
	size_t m_currentCommandIndex = 0;
};