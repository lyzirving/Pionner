#include "function/ui/WindowView.h"

#include "function/event/Event.h"

namespace Pionner
{
	WindowView::WindowView() : m_layout(), m_drawOrder(ORDER_NONE), m_uid(UID_NONE)
	{
	}

	WindowView::WindowView(uint8_t drawOrder) : m_layout(), m_drawOrder(drawOrder), m_uid(UID_NONE)
	{
	}

	WindowView::~WindowView()
	{
		m_parent.reset();
	}

	bool WindowView::dealEvent(RenderParam &param, const Event &evt)
	{
		if (m_layout.contains(evt.m_posX, evt.m_posY))
		{
			return processEvent(param, evt);
		}
		return false;
	}

	bool WindowView::processEvent(RenderParam &param, const Event &evt)
	{
		return false;
	}
}