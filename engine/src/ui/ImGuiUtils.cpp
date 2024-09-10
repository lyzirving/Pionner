#include "ImGuiUtils.h"

namespace pio
{
	const int32_t ImGuiUtils::k_FlagCollapseHeader = ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagSelectedNode = ImGuiTreeNodeFlags_OpenOnArrow |
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
}