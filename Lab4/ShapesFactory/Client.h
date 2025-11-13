#pragma once
#include "Designer.h"
#include "Painter.h"
#include <iostream>

class Client
{
public:
	Client(
		std::unique_ptr<IDesigner>&& designer,
		std::unique_ptr<Painter>&& painter,
		std::unique_ptr<ICanvas>&& canvas)
		: m_designer(move(designer))
		, m_painter(move(painter))
		, m_canvas(move(canvas)) {
		};

	void CreateDraft()
	{
		m_draft = m_designer->CreateDraft(std::cin);
	}

	void Draw()
	{
		if (!m_draft)
		{
			throw std::runtime_error("Draft wasn't created\n");
		}
		m_painter->DrawPicture(*m_draft, *m_canvas);
	}

private:
	std::unique_ptr<PictureDraft> m_draft;
	std::unique_ptr<IDesigner> m_designer;
	std::unique_ptr<Painter> m_painter;
	std::unique_ptr<ICanvas> m_canvas;
};