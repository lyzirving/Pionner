#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "ui/UiDef.h"
#include "core/InterfaceDef.h"
#include "window/event/EventHandler.h"

namespace pio
{
	enum EditorAxis : uint8_t
	{
		EditorAxis_X, EditorAxis_Y, EditorAxis_Z, EditorAxis_Num
	};

	class EditorUI : public Drawable, public EventHandler
	{
	public:
		EditorUI() {}
		virtual ~EditorUI() = default;

		bool bDrawOutline() const { return m_drawOutline; }

		void setDrawOutline(bool draw) { m_drawOutline = draw; }
		void setLayoutParam(const WindowLayoutParams &param) { m_layoutParam = param; }

	protected:
		bool m_drawOutline{ false };
		WindowLayoutParams m_layoutParam{};
	};
}

#endif