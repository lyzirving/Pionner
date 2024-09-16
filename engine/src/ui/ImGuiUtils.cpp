#include "ImGuiUtils.h"

#include "scene/node/Node.h"
#include "scene/Components.h"

namespace pio
{
	const int32_t ImGuiUtils::k_FlagCollapseHeader = ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagSelectedTreeNode = ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagTreeLeaf = ImGuiTreeNodeFlags_Leaf |
		ImGuiTreeNodeFlags_NoTreePushOnOpen;

	const int32_t ImGuiUtils::k_FlagCommonWindow = ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoScrollWithMouse | 
		ImGuiWindowFlags_NoCollapse;

	void ImGuiUtils::DrawImage(int32_t texId, const glm::vec2& imgSize, const glm::vec2& ltTexCoord, const glm::vec2& rbTexCoord, float rowWidth, float indent)
	{
		bool useIndent = !Math::IsZero(indent);
		bool useRowWidth = !Math::IsZero(rowWidth);

		if(useIndent)
		{
			ImGui::Indent(indent);
		}

		if(useRowWidth)
		{
			ImGui::PushItemWidth(rowWidth);
		}

		ImTextureID id = (int32_t*)texId;
		ImVec2 uv_min = ImVec2(ltTexCoord.x, ltTexCoord.y); // Top-left
		ImVec2 uv_max = ImVec2(rbTexCoord.x, rbTexCoord.y); // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
		ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
		ImGui::Image(id, ImVec2(imgSize.x, imgSize.y), uv_min, uv_max, tint_col, border_col);

		if(useRowWidth)
		{
			ImGui::PopItemWidth();
		}

		if(useIndent)
		{
			ImGui::Unindent(indent);
		}
	}

	void ImGuiUtils::HelpMaker(const char* msg)
	{
		ImGui::TextDisabled("(?)");
		if(ImGui::BeginItemTooltip())
		{
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(msg);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	bool ImGuiUtils::ItemBeingClicked()
	{
		return ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();
	}

	void ImGuiUtils::ShowHierarchy(const std::vector<Ref<Node>>& nodes, uint32_t& selectIdx)
	{
		if(nodes.empty())
			return;

		uint32_t clickIdx = InvalidId;
		if(selectIdx == InvalidId)
		{
			selectIdx = nodes[0]->idx();
		}
		ShowRelation(nodes, selectIdx, clickIdx);
	}

	void ImGuiUtils::ShowNode(const Ref<Node>& node)
	{
		auto type = node->nodeType();
		switch (type)
		{
			case pio::NodeType::Camera:
				DrawCameraPanel(node);
				break;
			default:
				break;
		}
	}

	void ImGuiUtils::ShowRelation(const std::vector<Ref<Node>>& nodes, uint32_t& selectIdx, uint32_t& clickIdx)
	{
		for(size_t i = 0; i < nodes.size(); i++)
		{
			ImGuiTreeNodeFlags tnFlags = ImGuiUtils::k_FlagSelectedTreeNode;
			tnFlags |= (selectIdx == nodes[i]->idx()) ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None;
			bool hasChild = !nodes[i]->children().empty();
			if(!hasChild)
			{
				tnFlags |= ImGuiUtils::k_FlagTreeLeaf;
			}
			bool bOpen = ImGui::TreeNodeEx(nodes[i]->name().c_str(), tnFlags);
			if(ImGuiUtils::ItemBeingClicked())
			{
				clickIdx = nodes[i]->idx();
			}
			if(bOpen && hasChild)
			{
				ShowRelation(nodes[i]->children(), selectIdx, clickIdx);
				ImGui::TreePop();
			}
			if(clickIdx != InvalidId)
			{
				selectIdx = clickIdx;
			}
		}
	}

	void ImGuiUtils::DrawCameraPanel(const Ref<Node>& node)
	{
		if(!node || !node->has<CameraComponent>())
		{
			return;
		}
		auto* cameraComp = node->getComponent<CameraComponent>();
		auto* transComp = node->getComponent<TransformComponent>();

		DrawTransformPanel(transComp);

		if(ImGui::CollapsingHeader("Camera", ImGuiUtils::k_FlagCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Near      ");
			ImGui::SameLine();
			ImGui::DragFloat("##Near", &cameraComp->Near, 1.f, 0.1, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Far       ");
			ImGui::SameLine();
			ImGui::DragFloat("##Far", &cameraComp->Far, 1.f, 0.1, 1000.f, "%.1f");

			int prjType{ cameraComp->PrjType };
			const char* items[2]{ "Perspective", "Orthographic" };
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Projection");
			ImGui::SameLine();
			ImGui::Combo("##Prj_type", &prjType, items, ProjectionType_Num);
			cameraComp->PrjType = ProjectionType(prjType);

			if(prjType == ProjectionType_Perspective)
			{				
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Fov       ");
				ImGui::SameLine();
				ImGui::DragFloat("##FOV", &cameraComp->Fov, 1.f, 0.1, 179.f, "%.1f");				
			}
			else
			{				
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Size      ");
				ImGui::SameLine();
				ImGui::DragFloat("##Size", &cameraComp->Size, 0.1f, 0.1, 100.f, "%.1f");
			}
		}
	}

	void ImGuiUtils::DrawTransformPanel(TransformComponent* comp)
	{
		if(ImGui::CollapsingHeader("Transform", ImGuiUtils::k_FlagCollapseHeader))
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", glm::value_ptr(comp->Position), 0.05f, -100.f, 100.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##Rotation", glm::value_ptr(comp->Rotation), 0.1f, -360.f, 360.f, "%.1f");

			ImGui::AlignTextToFramePadding();
			//Enough text padding for text alignment
			ImGui::Text("Scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Scale", glm::value_ptr(comp->Scale), 0.1f, 0.f, 10.f, "%.1f");
		}
	}
}