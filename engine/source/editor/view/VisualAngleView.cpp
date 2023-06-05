#include "view/VisualAngleView.h"
#include "function/ui/WindowUI.h"

#include "function/render/RenderDef.h"
#include "function/render/rhi/Rhi.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VisualAngleView"

namespace Pionner
{
	VisualAngleView::VisualAngleView() : WindowView(ORDER_VIEW_1)
	{
		m_uid = UID_VISUAL_ANGLE;
	}

	VisualAngleView::~VisualAngleView()
	{
	}

	void VisualAngleView::draw(RenderParam &param)
	{

	}

	void VisualAngleView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		if (!m_parent)
		{
			LOG_ERR("parent is invalid");
			return;
		}

		std::shared_ptr<WindowView> renderView{ nullptr };
		if (!(renderView = m_parent->getView(UID_RENDER_PORT)))
		{
			LOG_ERR("needed views are invalid");
			return;
		}

		ViewLayout renderLayout = renderView->getLayout();

		m_layout.m_width = renderLayout.m_width / 8;
		m_layout.m_height = m_layout.m_width;

		m_layout.m_left = renderLayout.m_left + renderLayout.m_width - m_layout.m_width;
		m_layout.m_top = renderLayout.m_top;
	}
}