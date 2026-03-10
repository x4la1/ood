#include "IDocument.h"
#include "ImageService.h"

class ICommand
{
public:
	virtual void Execute() = 0;
	virtual void Unexecute() = 0;
	virtual bool TryMerge(ICommand& command) = 0;
	virtual ~ICommand() = default;
};

class AbstractDocumentCommand : public ICommand
{
public:
	AbstractDocumentCommand(std::weak_ptr<IDocument> document)
		:m_document(document)
	{
	}

protected:
	IDocument& TryGetDocument()
	{
		auto doc = m_document.lock();
		if (!doc)
		{
			throw std::runtime_error("Document not exist");
		}

		return *doc;
	}

private:
	std::weak_ptr<IDocument> m_document;
};

class InsertParagraphCommand : public AbstractDocumentCommand
{
public:
	InsertParagraphCommand(std::weak_ptr<IDocument> document, const std::string& text, std::optional<size_t> index = std::nullopt)
		:AbstractDocumentCommand(document),
		m_text(text),
		m_index(index),
		m_actualIndex(0)
	{
	}

	void Execute() override
	{
		auto& doc = TryGetDocument();

		size_t actualIndex = m_index.value_or(doc.GetItemsCount());
		doc.InsertParagraph(m_text, actualIndex);

		m_actualIndex = actualIndex;
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();
		doc.DeleteItem(m_actualIndex);
	}

	bool TryMerge(ICommand& command) override
	{
		return false;
	}

private:
	std::string m_text;

	std::optional<size_t> m_index;
	size_t m_actualIndex = 0;
};

class InsertImageCommand : public AbstractDocumentCommand
{
public:
	InsertImageCommand(std::weak_ptr<IDocument> document, std::weak_ptr<ImageService> imageService, const std::string& sourcePath,
		uint32_t width, uint32_t height, std::optional<size_t> index = std::nullopt)
		:AbstractDocumentCommand(document),
		m_sourcePath(sourcePath),
		m_width(width),
		m_height(height),
		m_index(index),
		m_actualIndex(0),
		m_imageService(imageService)
	{
	}

	void Execute() override
	{
		auto& doc = TryGetDocument();
		auto service = m_imageService.lock();
		if (!service)
		{
			throw std::runtime_error("ImageService is not exist");
		}

		if (m_internalPath.empty())
		{
			m_internalPath = service->AddImage(m_sourcePath);
		}

		size_t actualIndex = m_index.value_or(doc.GetItemsCount());

		doc.InsertImage(m_internalPath, m_width, m_height, actualIndex);

		m_actualIndex = actualIndex;
		m_isDeleted = false;
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();
		doc.DeleteItem(m_actualIndex);
		m_isDeleted = true;
	}

	bool TryMerge(ICommand& command) override
	{
		return false;
	}

	~InsertImageCommand()
	{
		if (m_isDeleted && !m_internalPath.empty())
		{
			if (auto service = m_imageService.lock())
			{
				service->DeleteImage(m_internalPath);
			}
		}
	}

private:
	std::weak_ptr<ImageService> m_imageService;

	std::string m_sourcePath;
	std::string m_internalPath;

	uint32_t m_width;
	uint32_t m_height;

	std::optional<size_t> m_index;
	size_t m_actualIndex = 0;


	bool m_isDeleted = false;
};

class SetTitleCommand : public AbstractDocumentCommand
{
public:
	SetTitleCommand(std::weak_ptr<IDocument> document, const std::string& title)
		:AbstractDocumentCommand(document),
		m_newTitle(title)
	{

	}

	void Execute() override
	{
		auto& doc = TryGetDocument();

		if (m_oldTitle == std::nullopt)
		{
			m_oldTitle = doc.GetTitle();
		}

		doc.SetTitle(m_newTitle);
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();
		doc.SetTitle(m_oldTitle.value());
	}

	bool TryMerge(ICommand& command) override
	{
		auto* other = dynamic_cast<SetTitleCommand*>(&command);

		if (other)
		{
			m_newTitle = other->m_newTitle;
			return true;
		}

		return false;
	}

private:
	std::string m_newTitle;
	std::optional<std::string> m_oldTitle = std::nullopt;
};

class ReplaceTextCommand : public AbstractDocumentCommand
{
public:
	ReplaceTextCommand(std::weak_ptr<IDocument> document, const std::string& text, size_t index)
		:AbstractDocumentCommand(document),
		m_newText(text),
		m_index(index)
	{

	}

	void Execute() override
	{
		auto& doc = TryGetDocument();
		auto& item = doc.GetItem(m_index);

		if (auto paragraph = item.GetParagraph())
		{
			if (m_oldText == std::nullopt)
			{
				m_oldText = paragraph->GetText();
			}

			paragraph->SetText(m_newText);
		}
		else
		{
			throw std::runtime_error("Item is not a paragraph");
		}
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();
		auto item = doc.GetItem(m_index);

		if (auto paragraph = item.GetParagraph())
		{
			paragraph->SetText(m_oldText.value());
		}
	}

	bool TryMerge(ICommand& command) override
	{
		auto* other = dynamic_cast<ReplaceTextCommand*>(&command);

		if (other && other->m_index == m_index)
		{
			m_newText = other->m_newText;
			return true;
		}

		return false;
	}


private:
	std::string m_newText;
	std::optional<std::string> m_oldText = std::nullopt;
	size_t m_index;
};

class ResizeImageCommand : public AbstractDocumentCommand
{
public:
	ResizeImageCommand(std::weak_ptr<IDocument> document, size_t index, uint32_t width, uint32_t height)
		:AbstractDocumentCommand(document),
		m_index(index),
		m_newWidth(width),
		m_newHeight(height)
	{

	}

	void Execute() override
	{
		auto& doc = TryGetDocument();
		auto item = doc.GetItem(m_index);

		if (auto image = item.GetImage())
		{
			if (m_oldHeight == std::nullopt && m_oldWidth == std::nullopt)
			{
				m_oldWidth = image->GetWidth();
				m_oldHeight = image->GetHeight();
			}

			image->Resize(m_newWidth, m_newHeight);
		}
		else
		{
			throw std::runtime_error("Item is not an image");
		}
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();
		auto& item = doc.GetItem(m_index);

		if (auto image = item.GetImage())
		{
			image->Resize(m_oldWidth.value(), m_oldHeight.value());
		}
	}

	bool TryMerge(ICommand& command) override
	{
		auto* other = dynamic_cast<ResizeImageCommand*>(&command);

		if (other && other->m_index == m_index)
		{
			m_newWidth = other->m_newWidth;
			m_newHeight = other->m_newHeight;

			return true;
		}

		return false;
	}
private:
	size_t m_index;

	uint32_t m_newWidth;
	uint32_t m_newHeight;

	std::optional<uint32_t> m_oldWidth = std::nullopt;
	std::optional<uint32_t> m_oldHeight = std::nullopt;
};

class DeleteItemCommand : public AbstractDocumentCommand
{
public:
	DeleteItemCommand(std::weak_ptr<IDocument> document, size_t index, std::weak_ptr<ImageService> imageService)
		:AbstractDocumentCommand(document),
		m_index(index),
		m_imageService(imageService)
	{
	}

	void Execute() override
	{
		auto& doc = TryGetDocument();

		m_item = doc.GetItem(m_index);

		doc.DeleteItem(m_index);
		m_isDeleted = true;
	}

	void Unexecute() override
	{
		auto& doc = TryGetDocument();

		if (auto image = m_item.value().GetImage())
		{
			doc.InsertImage(image->GetPath(), image->GetWidth(), image->GetHeight(), m_index);
		}
		else if (auto paragraph = m_item.value().GetParagraph())
		{
			doc.InsertParagraph(paragraph->GetText(), m_index);
		}
	}

	bool TryMerge(ICommand& command) override
	{
		return false;
	}

	~DeleteItemCommand()
	{
		if (m_isDeleted)
		{
			if (auto image = m_item->GetImage())
			{
				if (auto service = m_imageService.lock())
				{
					service->DeleteImage(image->GetPath());
				}
			}
		}
	}

private:
	size_t m_index;
	std::optional<DocumentItem> m_item;
	bool m_isDeleted = false;
	std::weak_ptr<ImageService> m_imageService;
};