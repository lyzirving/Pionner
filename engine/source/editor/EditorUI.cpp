#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "EditorUI.h"

#include "function/render/RenderSystem.h"
#include "function/render/WindowSystem.h"
#include "function/render/scene/SceneMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EditorUI"

namespace Pionner
{
	EditorUI::EditorUI() : WindowUI()
	{
	}

	EditorUI::~EditorUI() = default;

	void EditorUI::initialize(WindowUIInitInfo &info)
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;

		ImGuiStyle &style = ImGui::GetStyle();
		setColorStyle(style);

		info.renderSystem->initializeUIRenderBackend(this);
	}

	void EditorUI::draw(std::shared_ptr<SceneMgr> &sceneMgr)
	{
		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
	}

	void EditorUI::shutdown()
	{
		ImGui::DestroyContext();
	}

	void EditorUI::setColorStyle(ImGuiStyle &style)
	{
		// to be implemented
	}
}