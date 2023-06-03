#ifndef __WINDOW_VIEW_H__
#define __WINDOW_VIEW_H__

#include <memory>

#include "function/ui/UiDef.h"

namespace Pionner
{
	struct RenderParam;
	class  WindowUI;

	class WindowView
	{
	public:
		WindowView();
		WindowView(uint8_t drawOrder);
		virtual ~WindowView();

		virtual void draw(RenderParam &param) = 0;
		virtual void layout(int32_t windowWidth, int32_t windowHeight) = 0;

		inline void setDrawOrder(uint8_t order) { m_drawOrder = order; }
		inline void setUid(uint8_t uid) { m_uid = uid; }

		inline void attachParent(const std::shared_ptr<WindowUI> &parent) { m_parent = parent; }
		inline uint8_t getDrawOrder() const { return m_drawOrder; }
		inline uint8_t getUid() const { return m_uid; }
		inline ViewLayout getLayout() const { return m_layout; }

	protected:
		ViewLayout                m_layout;
		uint8_t                   m_drawOrder;
		uint8_t                   m_uid;
		std::shared_ptr<WindowUI> m_parent;
	};
}

#endif