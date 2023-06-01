#ifndef __WINDOW_VIEW_H__
#define __WINDOW_VIEW_H__

#include <memory>

namespace Pionner
{
	class SceneMgr;

	enum ViewDrawOrder : uint8_t
	{
		ORDER_RENDER_PORT,
		ORDER_VIEW_1,
		ORDER_VIEW_2,
		ORDER_VIEW_3,
		ORDER_VIEW_4,
		ORDER_VIEW_5,
		ORDER_VIEW_6
	};

	struct ViewLayout
	{
		int32_t  m_left;
		int32_t  m_top;
		uint32_t m_width;
		uint32_t m_height;

		ViewLayout() : m_left(0), m_top(0), m_width(0), m_height(0) {}
	};

	class WindowView
	{
	public:
		WindowView();
		WindowView(uint8_t drawOrder);
		virtual ~WindowView();

		virtual void draw(std::shared_ptr<SceneMgr> &sceneMgr) = 0;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) = 0;

		inline void setDrawOrder(uint8_t order) { m_drawOrder = order; }

		inline uint8_t getDrawOrder() const { return m_drawOrder; }
		inline ViewLayout getLayout() const { return m_layout; }

	protected:
		ViewLayout m_layout;
		uint8_t    m_drawOrder;
	};
}

#endif