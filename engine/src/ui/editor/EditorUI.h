#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "core/InterfaceDef.h"

namespace pio
{
	enum EditorAxis : uint8_t
	{
		EditorAxis_X, EditorAxis_Y, EditorAxis_Z, EditorAxis_Cnt
	};

	class EditorUI : public Drawable
	{
	public:
		EditorUI() {}
		virtual ~EditorUI() = default;
	};
}

#endif