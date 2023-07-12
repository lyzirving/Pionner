#include "view/RenderView.h"

namespace pio
{
	RenderView::RenderView() : WindowView(ORDER_RENDER_PORT)
	{
		m_uid = UID_RENDER_PORT;
	}

	RenderView::~RenderView()
	{
	}

	void RenderView::draw(sgf::RenderInfo &info)
	{
		// no implementation in RenderView,
		// it is just a placeholder.
	}

	void RenderView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		m_layout.m_width = uint32_t(windowWidth * (2.f / 3.f)) + 1;
		m_layout.m_height = uint32_t(windowHeight * (2.f / 3.f)) + 1;
		m_layout.m_top = 0;
		m_layout.m_left = (windowWidth - m_layout.m_width) / 2 + 1;
	}
}