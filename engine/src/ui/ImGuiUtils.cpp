#include "ImGuiUtils.h"

#include <imgui.h>

namespace pio
{
    const int32_t ImGuiUtils::Flag_Collapse_Header = ImGuiTreeNodeFlags_DefaultOpen |
                                                     ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                                     ImGuiTreeNodeFlags_OpenOnArrow |
                                                     ImGuiTreeNodeFlags_SpanAvailWidth;

    const int32_t ImGuiUtils::Flag_Selected_TreeNode = ImGuiTreeNodeFlags_OpenOnArrow |
                                                       ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                                       ImGuiTreeNodeFlags_SpanAvailWidth;

    const int32_t ImGuiUtils::Flag_TreeLeaf = ImGuiTreeNodeFlags_Leaf |
                                              ImGuiTreeNodeFlags_NoTreePushOnOpen;

	void ImGuiUtils::HelpMaker(const char *msg)
	{
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip())
        {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(msg);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
	}

    bool ImGuiUtils::IsItemClicked()
    {
        return ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();
    }
}