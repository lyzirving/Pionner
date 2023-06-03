#include "function/ui/WindowView.h"

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
}