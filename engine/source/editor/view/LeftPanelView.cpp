#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "LeftPanelView.h"
#include "global/window/ui/WindowUI.h"
#include "gfx/context/GraphicContext.h"
#include "scenegraph/SceneGrfDef.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LeftPanelView"

namespace pio
{
	LeftPanelView::LeftPanelView() : WindowView(ORDER_VIEW_2)
	{
		m_uid = UID_LEFT_PANEL;
	}

	LeftPanelView::~LeftPanelView()
	{
	}

	void LeftPanelView::draw(sgf::RenderInfo &info)
	{
		auto rhi = info.gfxContext->getRhi();
		rhi->setViewport(0, 0, m_parent->getWndWidth(), m_parent->getWndHeight());

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
		if (!ImGui::Begin("editor_left", nullptr, windowFlags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

		//>>>>>> draw content 
		ImGui::CollapsingHeader("Entities");
		//>>>>>> finish content

		// End of show editor
		ImGui::PopItemWidth();
		ImGui::End();
	}

	void LeftPanelView::layout(int32_t windowWidth, int32_t windowHeight)
	{
		if (!m_parent)
		{
			LOG_ERR("parent is invalid");
			return;
		}

		std::shared_ptr<WindowView> renderView{ nullptr };
		if (!(renderView = m_parent->getView(UID_RENDER_PORT)))
		{
			LOG_ERR("render view is invalid");
			return;
		}

		ViewLayout layout = renderView->getLayout();

		m_layout.m_left = 0;
		m_layout.m_top = 0;
		m_layout.m_width = (windowWidth - layout.m_width) / 2;
		m_layout.m_height = layout.m_height;
	}
}