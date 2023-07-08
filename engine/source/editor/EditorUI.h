#ifndef __EDITOR_UI_H__
#define __EDITOR_UI_H__

#include "global/window/ui/WindowUI.h"

struct ImGuiStyle;

namespace pio
{
	class EditorUI : public WindowUI
	{
	public:
		EditorUI();
		virtual ~EditorUI();

		virtual void initialize(WindowUIInitInfo &info) override;
		virtual void draw(RenderParam &param) override;

		void createUI(const std::shared_ptr<WindowUI> &self);
		void shutdown();

	private:
		void setColorStyle(ImGuiStyle &style);

	};
}

#endif