#include "OutlinePanel.h"
#include "ui/imgui/ImGuiUtils.h"

#include "event/EventMgr.h"

#include "gfx/renderer/RenderContext.h"

#include "scene/Scene.h"
#include "scene/node/CameraNode.h"
#include "scene/node/DirectionalLitNode.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "OutlinePanel"

namespace pio
{
	OutlinePanel::OutlinePanel(const std::string& name) : ImGuiWindow(name)
	{
	}

	void OutlinePanel::OnDraw(const Ref<RenderContext>& context)
	{
		if(!IsSceneBound())
		{
			LOGE("scene is not bound");
			return;
		}
		auto lastSelect = m_Select;
		auto scene = GetScene();
		auto select = ShowHierarchy(scene, lastSelect);
		if(m_Select != select)
		{			
			if(m_Select != InvalidId)
				scene->FindNode(m_Select)->OnDeselect();
			if(select != InvalidId)
				scene->FindNode(select)->OnSelect();
			m_Select = select;
			EventMgr::Get()->BroadcastEvent(CreateRef<NodeSelectEvent>(m_Select));
		}
	}

	uint32_t OutlinePanel::ShowHierarchy(const Ref<Scene>& scene, uint32_t lastSelect)
	{
		if(!scene)
			return InvalidId;

		uint32_t select = lastSelect;
		const auto& nodes = scene->GetNodes();
		for(auto it : nodes)
		{
			ShowNodeHierarchy(it.second, select);
		}
		return select;
	}

	void OutlinePanel::ShowNodeHierarchy(const Ref<Node>& node, uint32_t& select)
	{
		if(!node->IsShowOutliner())
			return;

		if(select == InvalidId)
			select = node->UId();

		ImGuiTreeNodeFlags tnFlags = ImGuiUtils::k_FlagTreeNode;
		tnFlags |= (select == node->UId()) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;

		const auto& children = node->Children();
		bool hasChild = !children.empty();
		if(hasChild)
		{
			bool anyShow{ false };
			for(const auto& c : children)
				anyShow |= c->IsShowOutliner();
			hasChild = anyShow;
		}

		if(!hasChild)
			tnFlags |= ImGuiUtils::k_FlagTreeLeaf;

		bool bOpen = ImGui::TreeNodeEx(node->GetName().c_str(), tnFlags);
		uint32_t click = InvalidId;
		if(ImGuiUtils::ItemBeingClicked())
			click = node->UId();

		if(bOpen && hasChild)
		{
			const auto& children = node->Children();
			for(const auto& n : children)
				ShowNodeHierarchy(n, select);

			ImGui::TreePop();
		}

		if(click != InvalidId)
			select = click;
	}
}