#include <imgui.h>
#include <imgui_internal.h>

#include "EditorUI.h"

#include "function/render/RenderSystem.h"
#include "function/render/WindowSystem.h"

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

	void EditorUI::initialize(WindowUIInitInfo& info)
	{
		ImGui::CreateContext();
		info.renderSystem->initializeUIRenderBackend(this);
	}

	void EditorUI::preRender()
	{
	}

	void EditorUI::shutdown()
	{
		ImGui::DestroyContext();
	}
}