#pragma once

#include "ui/imgui/ImGuiWindow.h"

namespace pio
{
	class Scene;
	class Node;

	class OutlinePanel : public ImGuiWindow
	{
		RTTR_ENABLE(ImGuiWindow)
	public:
		OutlinePanel(const std::string& name = "Outliner");
		~OutlinePanel() = default;

		virtual void OnDraw(const Ref<RenderContext>& context) override;

	private:
		/*
		* @brief: show the hierarchy of nodes in the scene
		* @param lastSelect: Uid of the node which was selected
		* @return Uid of the node that is currently being selected
		*/
		uint32_t ShowHierarchy(const Ref<Scene>& scene, uint32_t lastSelect);
		void ShowNodeHierarchy(const Ref<Node>& node, uint32_t& select);

	private:
		uint32_t m_Select{ InvalidId }; 
	};
}