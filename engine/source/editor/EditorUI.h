#ifndef __EDITOR_UI_H__
#define __EDITOR_UI_H__

#include "function/ui/WindowUI.h"

struct ImGuiStyle;

namespace Pionner
{
	class EditorUI : public WindowUI
	{
	public:
		EditorUI();
		virtual ~EditorUI();

		virtual void initialize(WindowUIInitInfo &info) override;
		virtual void draw(RenderParam &param) override;

		void shutdown();

	private:
		void setColorStyle(ImGuiStyle &style);

	};
}

#endif