#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "view/BottomPanelView.h"
#include "window/ui/WindowUI.h"
#include "render/rhi/Rhi.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "BottomPanelView"

namespace pio
{
	BottomPanelView::BottomPanelView() : WindowView(ORDER_VIEW_4)
	{
		m_uid = UID_BOTTOM_PANEL;
	}

	BottomPanelView::~BottomPanelView()
	{
	}

	void BottomPanelView::draw(RenderParam &param)
	{
		param.rhi->setViewport(param.windowViewport.m_left, param.windowViewport.m_top,
							   param.windowViewport.m_width, param.windowViewport.m_height);

		ImGui::SetNextWindowPos(ImVec2(m_layout.m_left, m_layout.m_top),
								ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(m_layout.m_width, m_layout.m_height),
								 ImGuiCond_Always);

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;

		// Main body of the Demo window starts here.
		// Pass nullptr means no close button
		if (!ImGui::Begin("editor_bottom", nullptr, windowFlags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

		//>>>>>> draw content 

		//>>>>>> finish drawing content

		// End of show editor
		ImGui::PopItemWidth();
		ImGui::End();
	}

	void BottomPanelView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		if (!m_parent)
		{
			LOG_ERR("parent is invalid");
			return;
		}

		std::shared_ptr<WindowView> renderView{ nullptr };
		std::shared_ptr<WindowView> rightView{ nullptr };
		if (!(renderView = m_parent->getView(UID_RENDER_PORT)) || !(rightView = m_parent->getView(UID_RIGHT_PANEL)))
		{
			LOG_ERR("needed views are invalid");
			return;
		}

		ViewLayout renderLayout = renderView->getLayout();
		ViewLayout rightLayout = rightView->getLayout();

		m_layout.m_left = 0;
		m_layout.m_top = renderLayout.m_top + renderLayout.m_height;
		m_layout.m_width = windowWidth - rightLayout.m_width;
		m_layout.m_height = windowHeight - (renderLayout.m_height + renderLayout.m_top);
	}
}