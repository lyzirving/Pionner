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
		void setLayoutParam(const LayoutParams &param) { m_layoutParam = param; }

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

		static glm::vec4 GetAxisColor(EditorAxis axis, EditorAxis hover)
		{
			if (axis == hover)
				return glm::vec4(1.f);

			switch (axis)
			{
				case EditorAxis_Y:
					return glm::vec4(0.f, 1.f, 0.f, 1.f);
				case EditorAxis_Z:
					return glm::vec4(0.f, 0.f, 1.f, 1.f);
				case EditorAxis_X:
				default:
					return glm::vec4(1.f, 0.f, 0.f, 1.f);
			}
		}

	protected:
		bool m_bVisible{ false };
		bool m_bShowOutline{ false };
		LayoutParams m_layoutParam{};
	};
}

#endif