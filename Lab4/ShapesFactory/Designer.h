#pragma once
#include "PictureDraft.h"
#include "ShapeFactory.h"

class IDesigner
{
public:
	using DraftPtr = std::unique_ptr<PictureDraft>;
	virtual DraftPtr CreateDraft(std::istream& stream) = 0;
};

class Designer : public IDesigner
{
public:
	using FactoryPtr = std::unique_ptr<IShapeFactory>;

	Designer(FactoryPtr&& factory)
		: m_factory{ move(factory) }
	{
	}

	DraftPtr CreateDraft(std::istream& stream) override
	{
		auto draft = std::make_unique<PictureDraft>();
		std::string line;

		while (std::getline(stream, line))
		{
			if (line == "end")
			{
				break;
			}

			auto shape = m_factory->CreateShape(line);
			draft->AddShape(move(shape));
		}

		return draft;
	}

private:
	FactoryPtr m_factory;
};