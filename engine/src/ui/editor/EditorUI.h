#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "asset/Asset.h"

namespace pio
{
	enum EditorAxis : uint8_t
	{
		EditorAxis_X, EditorAxis_Y, EditorAxis_Z, EditorAxis_Cnt
	};

	class EditorUI
	{
	public:
		EditorUI() { onCreateMesh(); }
		virtual ~EditorUI() = default;

		virtual void onCreateMesh() = 0;
		virtual void onDraw() = 0;
	};
}

#endif