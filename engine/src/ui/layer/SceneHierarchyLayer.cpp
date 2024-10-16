#include "SceneHierarchyLayer.h"

#include "Application.h"

#include "scene/Scene.h"
#include "scene/Components.h"

#include "asset/AssetsManager.h"
#include "gfx/renderer/Renderer.h"

#include "ui/ImGuiUtils.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SceneHierarchyLayer"

namespace pio
{			
	static void ShowRelation(Registry *regi, const std::vector<uint32_t> &children, uint32_t &curSelect, uint32_t &selected)
	{
		if (children.empty())
			return;

		for (uint32_t i = 0; i < children.size(); i++)
		{
			Ref<Entity> child = regi->getCachedEntity(children[i]);
			auto &relation = child->getComponent<RelationshipComponent>();
			ImGuiTreeNodeFlags tnFlags = ImGuiUtils::Flag_Selected_TreeNode;
			tnFlags |= (selected == relation.Self) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			bool hasChild = relation.Children.size() != 0;
			if (!hasChild) { tnFlags |= ImGuiUtils::Flag_TreeLeaf; }
			bool bOpen = ImGui::TreeNodeEx(child->getName().c_str(), tnFlags);
			if (ImGuiUtils::bItemClicked()) { curSelect = relation.Self; }
			if (bOpen && hasChild)
			{
				ShowRelation(regi, relation.Children, curSelect, selected);
				ImGui::TreePop();
			}
		}
	}

	SceneHierarchyLayer::SceneHierarchyLayer(const LayoutParams &param)
		: Layer(param, "Scene Hierarchy Layer")
	{
	}

	void SceneHierarchyLayer::onAttach()
	{
		onWindowSizeChange(Application::MainWindow()->getWidth(),
						   Application::MainWindow()->getHeight());
	}

	void SceneHierarchyLayer::onDetach()
	{
	}

	void SceneHierarchyLayer::onUpdateUI(const Timestep &ts)
	{
		PIO_ASSERT_RETURN(Scene::Root.use_count() != 0, "SceneHierarchyLayer: scene root is invalid");

		Registry *regi = s_registry;
		Ref<Entity> rt = Scene::Root;
		LayoutParams &param = m_layoutParam;
		uint32_t &selected = m_selectedNode;
		s_registry->getSingleton<EditorComponent>().SelectedEntIndex = m_selectedNode;

		Renderer::SubmitRC([regi, rt, param, &selected]() mutable
		{
			PIO_ASSERT_RETURN(ImGui::GetCurrentContext() != nullptr, "SceneHierarchyLayer::onUpdate: Missing ImGui context");

			ImGuiWindowFlags flags = ImGuiWindowFlags_None;
			flags |= ImGuiWindowFlags_NoMove;
			flags |= ImGuiWindowFlags_NoResize;
			flags |= ImGuiWindowFlags_NoTitleBar;

			ImGui::SetNextWindowPos(ImVec2(param.Position.Left, param.Position.Top), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(param.Viewport.w() + 1, param.Viewport.h() + 1), ImGuiCond_Always);

			if (!ImGui::Begin("Scene Hierarchy", nullptr, flags))
			{
				// Early out if the window is collapsed, as an optimization.
				ImGui::End();
				return;
			}

			RelationshipComponent &rtRelation = rt->getComponent<RelationshipComponent>();
			ImGuiTreeNodeFlags tnFlags = ImGuiUtils::Flag_Selected_TreeNode | ImGuiTreeNodeFlags_DefaultOpen;
			if (selected == NullIndex) { selected = rtRelation.Self; }
			tnFlags |= (selected == rtRelation.Self) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			uint32_t curSelect{ NullIndex };
			bool bOpen = ImGui::TreeNodeEx(rt->getName().c_str(), tnFlags);
			if (ImGuiUtils::bItemClicked()) { curSelect = rtRelation.Self; }
			if (bOpen)
			{
				ShowRelation(regi, rtRelation.Children, curSelect, selected);
				ImGui::TreePop();
			}
			// Selection happens, update selection outside the loop
			if (curSelect != NullIndex) { selected = curSelect; }

			ImGui::End();
		});
	}

	void SceneHierarchyLayer::onWindowSizeChange(uint32_t width, uint32_t height)
	{
		m_layoutParam.calculate(width, height);
	}
}