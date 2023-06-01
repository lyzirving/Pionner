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
		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) override;

		void shutdown();

	private:
		void setColorStyle(ImGuiStyle &style);

	};
}

#endif