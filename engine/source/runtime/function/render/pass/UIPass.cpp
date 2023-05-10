#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "function/render/pass/UIPass.h"
#include "function/render/rhi/Rhi.h"

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
		m_ui = nullptr;
	}

	void UIPass::initialize(const RenderPassInitInfo &info)
	{
	}

	void UIPass::shutdown()
	{
	}

	void UIPass::initializeUIRenderBackend(WindowUI *ui)
	{
		m_ui = ui;
		m_rhi->initUIRenderBackend();
	}

	void UIPass::shutdownUIRenderBackend()
	{
		m_rhi->shutdownUIRenderBackend();
	}

	void UIPass::draw()
	{
		if (m_ui)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//>>>>>>do ui rendering
			//ImGui::ShowDemoWindow();
			//drawUI();
			//>>>>>>finish ui rendering

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}

	void UIPass::drawUI()
	{
		if (!ImGui::GetCurrentContext())
		{
			LOG_ERR("ImGui get current context is null");
			return;
		}
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_Always);

		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;
		windowFlags |= ImGuiWindowFlags_NoScrollbar;
		windowFlags |= ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoResize;

		// Main body of the Demo window starts here.
		// Pass nullptr means no close button
		if (!ImGui::Begin("PionnerEditor", nullptr, windowFlags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

		drawLeftDropdown();

		// End of show editor
		ImGui::PopItemWidth();
		ImGui::End();
	}

	void UIPass::drawLeftDropdown()
	{
		if (!ImGui::CollapsingHeader("Entities"))
			return;
	}
}
