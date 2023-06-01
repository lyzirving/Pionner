#include "function/ui/WindowView.h"

namespace Pionner
{
	WindowView::WindowView() : m_layout(), m_drawOrder(ORDER_RENDER_PORT)
	{
	}

	WindowView::WindowView(uint8_t drawOrder) : m_layout(), m_drawOrder(drawOrder)
	{
	}

	WindowView::~WindowView() = default;
}