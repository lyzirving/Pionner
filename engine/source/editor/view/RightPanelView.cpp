#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "view/RightPanelView.h"
#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RightPanelView"

namespace Pionner
{
	RightPanelView::RightPanelView() : WindowView(ORDER_VIEW_2)
	{
		m_uid = UID_RIGHT_PANEL;
	}

	RightPanelView::~RightPanelView()
	{
	}

	void RightPanelView::draw(RenderParam &param)
	{
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
		if (!ImGui::Begin("editor_right", nullptr, windowFlags))
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

	void RightPanelView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		if (!m_parent)
		{
			LOG_ERR("parent is invalid");
			return;
		}

		std::shared_ptr<WindowView> leftView{ nullptr };
		std::shared_ptr<WindowView> renderView{ nullptr };
		if (!(renderView = m_parent->getView(UID_RENDER_PORT)) || !(leftView = m_parent->getView(UID_LEFT_PANEL)))
		{
			LOG_ERR("needed views are invalid");
			return;
		}

		ViewLayout renderLayout = renderView->getLayout();
		ViewLayout leftLayout = leftView->getLayout();

		m_layout.m_left = leftLayout.m_left + leftLayout.m_width + renderLayout.m_width;
		m_layout.m_top = 0;
		m_layout.m_width = windowWidth - leftLayout.m_width - renderLayout.m_width;
		m_layout.m_height = windowHeight;
	}
}