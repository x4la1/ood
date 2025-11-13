#pragma once
#include "PictureDraft.h"

class Painter
{
public:
	void DrawPicture(PictureDraft& draft, ICanvas& canvas)
	{
		for (size_t i = 0; i < draft.GetShapeCount(); ++i)
		{
			draft.GetShape(i).Draw(canvas);
		}
	}
};
