#ifndef __WINDOW_VIEW_H__
#define __WINDOW_VIEW_H__

#include <memory>

#include "UiDef.h"

namespace pio
{
	struct RenderParam;
	class  WindowUI;
	class  Event;

	class WindowView
	{
	public:
		WindowView();
		WindowView(uint8_t drawOrder);
		virtual ~WindowView();

		virtual void draw(RenderParam &param) = 0;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) = 0;
		virtual bool processEvent(RenderParam &param, const Event &evt);
		virtual void resetMotion();

		inline void setDrawOrder(uint8_t order) { m_drawOrder = order; }
		inline void setUid(uint8_t uid) { m_uid = uid; }

		inline void attachParent(const std::shared_ptr<WindowUI> &parent) { m_parent = parent; }
		inline uint8_t getDrawOrder() const { return m_drawOrder; }
		inline uint8_t getUid() const { return m_uid; }
		inline ViewLayout getLayout() const { return m_layout; }

		bool dealEvent(RenderParam &param, const Event &evt);

	protected:
		ViewLayout                m_layout;
		uint8_t                   m_drawOrder;
		uint8_t                   m_uid;
		std::shared_ptr<WindowUI> m_parent;
	};
}

#endif