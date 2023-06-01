#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "function/render/pass/UIPass.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/scene/SceneMgr.h"

#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UIPass"

namespace Pionner
{
	UIPass::UIPass() : RenderPassBase()
		, m_ui(nullptr)
	{
	}

	UIPass::~UIPass()
	{
		m_ui.reset();
	}

	void UIPass::initialize(const RenderPassInitInfo &info)
	{
	}

	void UIPass::shutdown()
	{
		m_ui.reset();
		m_rhi.reset();
	}

	ViewLayout UIPass::getRenderportLayout()
	{
		if (m_ui)
		{
			return m_ui->getRenderportLayout();
		}
		else
		{
			LOG_ERR("ui ptr is invalid");
			return ViewLayout();
		}
	}

	void UIPass::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
	{
		m_ui = ui;
		m_rhi->initUIRenderBackend();
	}

	void UIPass::shutdownUIRenderBackend()
	{
		m_rhi->shutdownUIRenderBackend();
	}

	void UIPass::resize(int width, int height)
	{
		if (m_ui)
			m_ui->resize(width, height);
	}

	void UIPass::draw(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		if (m_ui)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			m_ui->draw(sceneMgr);
			drawUI(sceneMgr);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void UIPass::drawUI(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		if (!ImGui::GetCurrentContext())
		{
			LOG_ERR("ImGui get current context is null");
			return;
		}
		drawLeftPanel(sceneMgr);
		drawRightPanel(sceneMgr);
		drawBottomPanel(sceneMgr);
	}

	void UIPass::drawLeftPanel(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		auto layout = sceneMgr->m_layout;
		LayoutInfo &info = layout->m_leftPanelInfo;

		ImGui::SetNextWindowPos(ImVec2(info.m_left, info.m_top),
								ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(info.m_width, info.m_height),
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

	void UIPass::drawRightPanel(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		auto layout = sceneMgr->m_layout;
		LayoutInfo &info = layout->m_rightPanelInfo;

		ImGui::SetNextWindowPos(ImVec2(info.m_left, info.m_top),
								ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(info.m_width, info.m_height),
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

	void UIPass::drawBottomPanel(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		auto layout = sceneMgr->m_layout;
		LayoutInfo &info = layout->m_bottomPanelInfo;

		ImGui::SetNextWindowPos(ImVec2(info.m_left, info.m_top),
								ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(info.m_width, info.m_height),
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
}
