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

		bool bVisible() const { return m_bVisible; }
		bool bShowOutline() const { return m_bShowOutline; }

		void setVisible(bool val) { m_bVisible = val; }
		void setShowOutline(bool draw) { m_bShowOutline = draw; }
		void setLayoutParam(const WindowLayoutParams &param) { m_layoutParam = param; }

		static const char *EditorAxisStr(EditorAxis axis)
		{
			switch (axis)
			{
				case EditorAxis_X:
					return "Editor Axis X";
				case EditorAxis_Y:
					return "Editor Axis Y";
				case EditorAxis_Z:
					return "Editor Axis Z";
				default:
					return "Editor Axis Num";
			}
		}

		static const glm::vec3 &GetAxis(EditorAxis axis)
		{
			switch (axis)
			{
				case EditorAxis_Y:
					return AXIS_Y;
				case EditorAxis_Z:
					return AXIS_Z;
				case EditorAxis_X:
				case EditorAxis_Num:
				default:
					return AXIS_X;
			}
		}

	protected:
		bool m_bVisible{ false };
		bool m_bShowOutline{ false };
		WindowLayoutParams m_layoutParam{};
	};
}

#endif