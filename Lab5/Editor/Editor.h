#pragma once
#include "Menu.h"
#include "History.h"
#include "IDocument.h"


class Editor
{
public:
	Editor()
	{
		CreateCommands();
	}

	void Run()
	{
		m_menu.Run();
	}

	static size_t StringToPosition(const std::string& pos)
	{
		try
		{
			return std::stoi(pos);
		}
		catch (std::exception&)
		{
			throw std::runtime_error("Invalid position format");
		}
	}

private:
	Menu m_menu = Menu(std::cin, std::cout);
	History m_history;
	std::shared_ptr<IDocument> m_document = std::make_shared<HtmlDocument>();
	std::shared_ptr<ImageService> m_imageService = std::make_shared<ImageService>("temp");

	void CreateCommands()
	{
		m_menu.AddItem(
			"InsertParagraph",
			"Usage: InsertParagraph <position>|end <text>",
			[&](std::istringstream& input) {OnInsertParagraph(input); }
		);

		m_menu.AddItem(
			"InsertImage",
			"Usage: InsertImage <pos>|end <width> <height> <image path>",
			[&](std::istringstream& input) {OnInsertImage(input); }
		);

		m_menu.AddItem(
			"SetTitle",
			"Usage: SetTitle <title>",
			[&](std::istringstream& input) {OnSetTitle(input); }
		);

		m_menu.AddItem(
			"List",
			"Usage: List",
			[&](std::istringstream& input) {OnList(input); }
		);

		m_menu.AddItem(
			"ReplaceText",
			"Usage: ReplaceText <pos> <text>",
			[&](std::istringstream& input) {OnReplaceText(input); }
		);

		m_menu.AddItem(
			"ResizeImage",
			"Usage: ResizeImage <pos> <width> <height>",
			[&](std::istringstream& input) {OnResizeImage(input); }
		);

		m_menu.AddItem(
			"DeleteItem",
			"Usage: DeleteItem <pos>",
			[&](std::istringstream& input) {OnDeleteItem(input); }
		);

		m_menu.AddItem(
			"Help",
			"Usage: Help",
			[&](std::istringstream& input) {OnHelp(input); }
		);

		m_menu.AddItem(
			"Undo",
			"Usage: Undo",
			[&](std::istringstream& input) {OnUndo(input); }
		);

		m_menu.AddItem(
			"Redo",
			"Usage: Redo",
			[&](std::istringstream& input) {OnRedo(input); }
		);

		m_menu.AddItem(
			"Save",
			"Usage: Save <path>",
			[&](std::istringstream& input) {OnSave(input); }
		);

		m_menu.AddItem(
			"Exit",
			"Usage: Exit",
			[&](std::istringstream& input) {OnExit(input); }
		);
	}

	void OnInsertParagraph(std::istringstream& input)
	{
		std::string position, text;

		if (!(input >> position))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		if (!std::getline(input >> std::ws, text) || text.empty())
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		std::unique_ptr<InsertParagraphCommand> command;

		if (position == "end")
		{
			command = std::make_unique<InsertParagraphCommand>(m_document, text, std::nullopt);
		}
		else
		{
			command = std::make_unique<InsertParagraphCommand>(m_document, text, StringToPosition(position));
		}

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnInsertImage(std::istringstream& input)
	{
		std::string position, path;
		uint32_t width, height;

		if (!(input >> position >> width >> height >> path))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		std::unique_ptr<InsertImageCommand> command;

		if (position == "end")
		{
			command = std::make_unique<InsertImageCommand>(m_document, m_imageService, path, width, height);
		}
		else
		{
			command = std::make_unique<InsertImageCommand>
				(m_document, m_imageService, path, width, height, StringToPosition(position));
		}

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnSetTitle(std::istringstream& input)
	{
		std::string title;

		if (!std::getline(input >> std::ws, title) || title.empty())
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		auto command = std::make_unique<SetTitleCommand>(m_document, title);

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnList(std::istringstream&)
	{
		m_document->List(std::cout);
	}

	void OnReplaceText(std::istringstream& input)
	{
		std::string text;
		size_t position;

		if (!(input >> position))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		if (!std::getline(input >> std::ws, text) || text.empty())
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		auto command = std::make_unique<ReplaceTextCommand>(m_document, text, position);

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnResizeImage(std::istringstream& input)
	{
		size_t position;
		uint32_t width, height;

		if (!(input >> position >> width >> height))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		auto command = std::make_unique<ResizeImageCommand>(m_document, position, width, height);

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnDeleteItem(std::istringstream& input)
	{
		size_t position;

		if (!(input >> position))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		auto command = std::make_unique<DeleteItemCommand>(m_document, position, m_imageService);

		m_history.AddAndExecuteCommand(std::move(command));
	}

	void OnHelp(std::istringstream&)
	{
		m_menu.ShowInstructions();
	}

	void OnUndo(std::istringstream&)
	{
		m_history.Undo();
	}

	void OnRedo(std::istringstream&)
	{
		m_history.Redo();
	}

	void OnSave(std::istringstream& input)
	{
		std::string path;

		if (!(input >> path))
		{
			std::cout << "Invalid arguments\n";
			return;
		}

		m_document->Save(path);
	}

	void OnExit(std::istringstream& input)
	{
		m_menu.Exit();
	}
};