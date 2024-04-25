#ifndef __PIONNER_UI_EDITOR_EDITOR_UI_H__
#define __PIONNER_UI_EDITOR_EDITOR_UI_H__

#include "ui/UiDef.h"
#include "core/func/Drawable.h"
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

		virtual bool bSelected() const = 0;

		bool bVisible() const { return m_bVisible; }
		bool bShowOutline() const { return m_bShowOutline; }

		void setVisible(bool val) { m_bVisible = val; }
		void setShowOutline(bool draw) { m_bShowOutline = draw; }
		void setLayoutParam(const WindowLayoutParams &param) { m_layoutParam = param; }

	protected:
		bool m_bVisible{ false };
		bool m_bShowOutline{ false };
		WindowLayoutParams m_layoutParam{};
	};
}

#endif