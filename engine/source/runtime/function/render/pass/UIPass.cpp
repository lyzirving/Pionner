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
			//>>>>>>finish ui rendering

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}
