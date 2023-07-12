#ifndef __PIONNER_GLOBAL_WINDOW_UI_H__
#define __PIONNER_GLOBAL_WINDOW_UI_H__

#include <memory>
#include <vector>
#include <map>

#include "WindowView.h"

namespace pio
{
	struct RenderParam;
	class  WindowView;
	class  RenderParam;
	class  Event;

	struct WindowUIInitInfo
	{
		int32_t	wndWidth{ 0 };
		int32_t windHeight{ 0 };
	};

	namespace sgf
	{
		struct RenderInfo;
	}

	class WindowUI : public std::enable_shared_from_this<WindowUI>
	{
	public:
		WindowUI();
		virtual ~WindowUI();

		virtual void initialize(WindowUIInitInfo &info) = 0;
		virtual void draw(sgf::RenderInfo &info) = 0;
		virtual bool dealEvent(RenderParam &param, const Event &evt);

		void addView(const std::shared_ptr<WindowView> &view);
		bool contain(const std::shared_ptr<WindowView> &view);
		void layout();
		void resetTargetView();
		void setWndSize(int width, int height);

		std::shared_ptr<WindowUI>   selfPtr();
		std::shared_ptr<WindowView> getView(uint8_t uid);

		inline int32_t getWndWidth()  { return m_windowWidth; }
		inline int32_t getWndHeight() { return m_windowHeight; }

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