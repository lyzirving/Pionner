#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "gfx/renderer/RenderData.h"

namespace pio
{
	enum EditorAxis : uint8_t
	{
		EditorAxis_X, EditorAxis_Y, EditorAxis_Z, EditorAxis_Cnt
	};

	class EditorUI
	{
	public:
		EditorUI() {}
		virtual ~EditorUI() = default;

		virtual void onCreateMesh() = 0;
		virtual void onDraw(const DrawParam &param) = 0;
	};
}

#endif