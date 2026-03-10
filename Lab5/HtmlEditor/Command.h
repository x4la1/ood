#pragma once
#include "Document.h"
#include <memory>

class ICommand
{
public:
	virtual void Execute() = 0;
	~ICommand() = default;
};

class DocumentCommand : public ICommand
{
public:
	DocumentCommand(const std::weak_ptr<IDocument>& document)
		: m_document(document)
	{
	}

	std::shared_ptr<IDocument> TryGetDocument() const
	{
		auto doc = m_document.lock();
		if (!doc)
		{
			throw std::runtime_error("Document not exist");
		}
		return doc;
	}

private:
	std::weak_ptr<IDocument> m_document;
};

class UndoableCommand : public DocumentCommand
{
public:
	UndoableCommand(const std::weak_ptr<IDocument>& document)
		: DocumentCommand(document)
	{
	}

	virtual void UnExecute() = 0;

protected:
	bool m_isExecuted = false;
};

class MergableCommand : public UndoableCommand
{
public:
	MergableCommand(const std::weak_ptr<IDocument>& document)
		: UndoableCommand(document)
	{
	}

	virtual bool Merge(const std::shared_ptr<MergableCommand>& otherCommand) = 0;
};

class HelpCommand : public ICommand //TODO
{
public:
	void Execute() override
	{
		std::cout << "TODO\n";
	}
};

class UndoCommand : public DocumentCommand
{
public:
	UndoCommand(const std::weak_ptr<IDocument>& document)
		: DocumentCommand(document)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (doc->CanUndo())
		{
			doc->Undo();
		}
	}
};

class RedoCommand : public DocumentCommand
{
public:
	RedoCommand(const std::weak_ptr<IDocument>& document)
		: DocumentCommand(document)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (doc->CanRedo())
		{
			doc->Redo();
		}
	}
};

class SaveCommand : public DocumentCommand
{
public:
	SaveCommand(const std::weak_ptr<IDocument>& document, const Path& path)
		: DocumentCommand(document)
		, m_path(path)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();

		doc->Save(m_path);
	}

private:
	Path m_path;
};

class ListCommand : public DocumentCommand
{
public:
	ListCommand(const std::weak_ptr<IDocument>& document)
		: DocumentCommand(document)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		doc->List();
	}
};

class DeleteCommand : public UndoableCommand // TODO
{
public:
	DeleteCommand(const std::weak_ptr<IDocument>& document, const size_t& position)
		: UndoableCommand(document)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (!m_isExecuted)
		{
			auto item = doc->GetItem(m_position);
		}
	}

	void UnExecute() override
	{
		auto doc = TryGetDocument();
		auto item = doc->GetItem(m_position);
	}

	~DeleteCommand()
	{
		// TODO
	}

private:
	size_t m_position;
};

class InsertImageCommand : public UndoableCommand
{
public:
	InsertImageCommand(const std::weak_ptr<IDocument>& document,
		const Path& path,
		uint32_t width,
		uint32_t height,
		const std::optional<size_t>& position)
		: UndoableCommand(document)
		, m_path(path)
		, m_width(width)
		, m_height(height)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (!m_isExecuted)
		{
			doc->InsertImage(m_path, m_width, m_height, m_position);
			m_isExecuted = true;
		}
		else
		{
			auto item = doc->GetItem(m_position.value_or(doc->GetItemsCount() - 1));
			item.SetUnDelete();
		}
	}

	void UnExecute() override
	{
		if (m_isExecuted)
		{
			auto doc = TryGetDocument();
			auto item = doc->GetItem(m_position.value_or(doc->GetItemsCount() - 1));
			item.SetDelete();
		}
	}

	~InsertImageCommand()
	{
		// TODO
	}

private:
	Path m_path;
	uint32_t m_width;
	uint32_t m_height;
	std::optional<size_t> m_position;
};

class InsertParagraphCommand : public UndoableCommand
{
public:
	InsertParagraphCommand(const std::weak_ptr<IDocument>& document, const std::string& text, const std::optional<size_t>& position)
		: UndoableCommand(document)
		, m_text(text)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (!m_isExecuted)
		{
			doc->InsertParagraph(m_text, m_position);
			m_isExecuted = true;
		}
		else
		{
			auto item = doc->GetItem(m_position.value_or(doc->GetItemsCount() - 1));
			item.SetUnDelete();
		}
	}

	void UnExecute() override
	{
		if (m_isExecuted)
		{
			auto doc = TryGetDocument();
			auto item = doc->GetItem(m_position.value_or(doc->GetItemsCount() - 1));
			item.SetDelete();
		}
	}

	~InsertParagraphCommand()
	{
		// TODO
	}

private:
	std::string m_text;
	std::optional<size_t> m_position;
};

class SetTitleCommand : public MergableCommand
{
public:
	SetTitleCommand(const std::weak_ptr<IDocument>& document, const std::string& newText)
		: MergableCommand(document)
		, m_newText(newText)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		if (!m_isExecuted)
		{
			m_oldText = doc->GetTitle();
		}

		doc->SetTitle(m_newText);
	}

	void UnExecute() override
	{
		if (m_isExecuted)
		{
			auto doc = TryGetDocument();
			doc->SetTitle(m_oldText);
		}
	}

	bool Merge(const std::shared_ptr<MergableCommand>& otherCommand) override
	{
		if (auto other = std::dynamic_pointer_cast<SetTitleCommand>(otherCommand);
			other && other->m_oldText == m_newText)
		{
			m_newText = other->m_newText;
			auto doc = TryGetDocument();
			doc->SetTitle(m_newText);
			return true;
		}

		return false;
	}

private:
	std::string m_newText;
	std::string m_oldText;
};

class ResizeImageCommand : public MergableCommand
{
public:
	ResizeImageCommand(const std::weak_ptr<IDocument>& document,
		const uint32_t& newWidth,
		const uint32_t& newHeight,
		const size_t& position)
		: MergableCommand(document)
		, m_newWidth(newWidth)
		, m_newHeight(newHeight)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		auto& docItem = doc->GetItem(m_position);
		auto image = docItem.GetImage();
		if (!image)
		{
			throw std::runtime_error("No image at this position");
		}

		if (!m_isExecuted)
		{
			m_oldWidth = image->GetWidth();
			m_oldHeight = image->GetHeight();
			m_isExecuted = true;
		}

		image->Resize(m_newWidth, m_newHeight);
	}

	void UnExecute() override
	{
		if (m_isExecuted)
		{
			auto doc = TryGetDocument();
			auto& docItem = doc->GetItem(m_position);
			auto image = docItem.GetImage();

			if (image)
			{
				image->Resize(m_oldWidth, m_oldHeight);
			}
		}
	}

	bool Merge(const std::shared_ptr<MergableCommand>& otherCommand) override
	{
		if (auto other = std::dynamic_pointer_cast<ResizeImageCommand>(otherCommand);
			other && other->m_position == m_position && other->m_oldHeight == m_newHeight && other->m_oldWidth == m_newWidth)
		{
			m_newWidth = other->m_newWidth;
			m_newHeight = other->m_newHeight;
			auto doc = TryGetDocument();
			auto& docItem = doc->GetItem(m_position);
			auto image = docItem.GetImage();

			if (image)
			{
				image->Resize(m_newWidth, m_newHeight);
				return true;
			}
		}
		return false;
	}

private:
	uint32_t m_newWidth;
	uint32_t m_newHeight;
	uint32_t m_oldWidth;
	uint32_t m_oldHeight;
	size_t m_position;
};

class ReplaceTextCommand : public MergableCommand
{
public:
	ReplaceTextCommand(const std::weak_ptr<IDocument>& document, const std::string& newText, const size_t& position)
		: MergableCommand(document)
		, m_newText(newText)
		, m_position(position)
	{
	}

	void Execute() override
	{
		auto doc = TryGetDocument();
		auto& docItem = doc->GetItem(m_position);
		auto paragraph = docItem.GetParagraph();

		if (!paragraph)
		{
			throw std::runtime_error("No paragraph in this position");
		}

		if (!m_isExecuted)
		{
			m_oldText = paragraph->GetText();
			m_isExecuted = true;
		}

		paragraph->SetText(m_newText);
	}

	void UnExecute() override
	{
		if (m_isExecuted)
		{
			auto doc = TryGetDocument();
			auto& docItem = doc->GetItem(m_position);
			auto paragraph = docItem.GetParagraph();

			if (paragraph)
			{
				paragraph->SetText(m_oldText);
			}
		}
	}

	bool Merge(const std::shared_ptr<MergableCommand>& otherCommand) override
	{
		if (auto other = std::dynamic_pointer_cast<ReplaceTextCommand>(otherCommand);
			other && other->m_position == m_position && other->m_oldText == m_newText)
		{
			m_newText = other->m_newText;
			auto doc = TryGetDocument();
			auto& docItem = doc->GetItem(m_position);
			auto paragraph = docItem.GetParagraph();

			if (paragraph)
			{
				paragraph->SetText(m_newText);
				return true;
			}
		}
		return false;
	}

private:
	std::string m_newText;
	std::string m_oldText;
	size_t m_position;
};