#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "EditorUI.h"

#include "view/RenderView.h"
#include "view/VisualAngleView.h"
#include "view/LeftPanelView.h"
#include "view/RightPanelView.h"
#include "view/BottomPanelView.h"

#include "render/RenderSystem.h"
#include "window/WindowSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EditorUI"

namespace pio
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

		m_windowWidth = info.windowSystem->getWidth();
		m_windowHeight = info.windowSystem->getHeight();
	}

	void EditorUI::draw(RenderParam &param)
	{
		sortView();

		if (!ImGui::GetCurrentContext())
		{
			LOG_ERR("ImGui get current context is null");
			return;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for (size_t i = 0; i < m_viewArray.size(); i++)
		{
			if (m_viewArray[i].second)
				m_viewArray[i].second->draw(param);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUI::createUI(const std::shared_ptr<WindowUI> &self)
	{
		std::shared_ptr<WindowView> render = std::shared_ptr<WindowView>(new RenderView);
		render->attachParent(self);
		std::shared_ptr<WindowView> visualAngle = std::shared_ptr<WindowView>(new VisualAngleView);
		visualAngle->attachParent(self);
		std::shared_ptr<WindowView> leftPanel = std::shared_ptr<WindowView>(new LeftPanelView);
		leftPanel->attachParent(self);
		std::shared_ptr<WindowView> rightPanel = std::shared_ptr<WindowView>(new RightPanelView);
		rightPanel->attachParent(self);
		std::shared_ptr<WindowView> bottomPanel = std::shared_ptr<WindowView>(new BottomPanelView);
		bottomPanel->attachParent(self);

		addView(render);
		addView(visualAngle);
		addView(leftPanel);
		addView(rightPanel);
		addView(bottomPanel);
	}

	void EditorUI::shutdown()
	{
		ImGui::DestroyContext();

		auto itr = m_viewArray.begin();
		while (itr != m_viewArray.end())
		{
			(*itr).second.reset();
			itr = m_viewArray.erase(itr);
		}

		auto itr1 = m_viewMap.begin();
		while (itr1 != m_viewMap.end())
		{
			itr1->second.reset();
			itr1 = m_viewMap.erase(itr1);
		}
	}

	void EditorUI::setColorStyle(ImGuiStyle &style)
	{
		// to be implemented
	}
}