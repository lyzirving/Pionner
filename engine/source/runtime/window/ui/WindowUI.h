#ifndef __WINDOW_UI_H__
#define __WINDOW_UI_H__

#include <memory>
#include <vector>
#include <map>

#include "WindowView.h"

namespace pio
{
	struct RenderParam;
	class  WindowSystem;
	class  RenderSystem;
	class  WindowView;
	class  RenderParam;
	class  Event;

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
		virtual void draw(RenderParam &param) = 0;
		virtual bool dealEvent(RenderParam &param, const Event &evt);

		void addView(const std::shared_ptr<WindowView> &view);
		bool contain(const std::shared_ptr<WindowView> &view);
		void layout();
		void resetTargetView();

		std::shared_ptr<WindowUI>   getPtr();
		std::shared_ptr<WindowView> getView(uint8_t uid);

		void resize(int width, int height);

	protected:
		typedef std::pair<uint8_t, std::shared_ptr<WindowView>> ViewItem;

		static bool pairSorter(const ViewItem &lhs, const ViewItem &rhs);

		void sortView();

	protected:
		std::map<uint8_t, std::shared_ptr<WindowView>> m_viewMap;
		std::vector<ViewItem>                          m_viewArray;
		std::shared_ptr<WindowView>                    m_target;
		int32_t m_windowWidth, m_windowHeight;
		bool    m_needSortView;
	};
}

#endif