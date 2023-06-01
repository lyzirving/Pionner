#ifndef __WINDOW_UI_H__
#define __WINDOW_UI_H__

#include <vector>

#include "function/ui/WindowView.h"

namespace Pionner
{
	class WindowSystem;
	class RenderSystem;
	class SceneMgr;

	struct WindowUIInitInfo
	{
		std::shared_ptr<WindowSystem> windowSystem;
		std::shared_ptr<RenderSystem> renderSystem;
	};

	class WindowUI : public std::enable_shared_from_this<WindowUI>
	{
	public:
		WindowUI();
		virtual ~WindowUI();

		virtual void initialize(WindowUIInitInfo &info) = 0;
		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) = 0;

		std::shared_ptr<WindowUI> getPtr();
		ViewLayout getRenderportLayout();

		void resize(int width, int height);

	protected:
		static bool viewSorter(std::shared_ptr<WindowView> &lhs, std::shared_ptr<WindowView> &rhs);

		void sortView();

	protected:
		std::vector<std::shared_ptr<WindowView>> m_views;
		int32_t m_windowWidth, m_windowHeight;
		bool    m_needSortView;
	};
}

#endif