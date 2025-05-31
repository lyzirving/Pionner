#include "ImGuiWindow.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImGuiWindow"

namespace pio
{
	const int32_t ImGuiWindow::k_CommonFlags = ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse;

	int32_t ImGuiWindow::k_InstNum = 0;

	ImGuiWindow::ImGuiWindow(const std::string& name) : 
		ImGuiWidget(name.empty() ? std::string("ImGuiWindow_").append(std::to_string(k_InstNum)) : name)
	{
		++k_InstNum;
	}

	ImGuiWindow::~ImGuiWindow()
	{
		--k_InstNum;
	}

	void ImGuiWindow::OnBeforeDraw(const Ref<RenderContext>& context)
	{
		if(IsDirty())
		{
			ImGui::SetNextWindowPos(ImVec2(Left(), Top()));
			ImGui::SetNextWindowSize(ImVec2(Width(), Height()));
		}
		ImGui::Begin(m_Name.c_str(), 0, k_CommonFlags | m_WindowFlags);
	}

	void ImGuiWindow::OnPostDraw(const Ref<RenderContext>& context)
	{
		ImGuiWidget::OnPostDraw(context);
		ImGui::End();
	}

	void ImGuiWindow::EnableMove(bool val)
	{
		if(val)
		{
			m_WindowFlags &= ~ImGuiWindowFlags_NoMove;
		}
		else
		{
			m_WindowFlags |= ImGuiWindowFlags_NoMove;
		}
	}

	void ImGuiWindow::EnableDocking(bool val)
	{
		if(val)
		{
			m_WindowFlags &= ~ImGuiWindowFlags_NoDocking;
		}
		else
		{
			m_WindowFlags |= ImGuiWindowFlags_NoDocking;
		}
	}

	void ImGuiWindow::EnableResize(bool val)
	{
		if(val)
		{
			m_WindowFlags &= ~ImGuiWindowFlags_NoResize;
		}
		else
		{
			m_WindowFlags |= ImGuiWindowFlags_NoResize;
		}
	}

	void ImGuiWindow::EnableTitle(bool val)
	{
		if(val)
		{
			m_WindowFlags &= ~ImGuiWindowFlags_NoTitleBar;
		}
		else
		{
			m_WindowFlags |= ImGuiWindowFlags_NoTitleBar;
		}
	}
}