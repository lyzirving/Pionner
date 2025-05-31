#include "ImGuiUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImGuiUtils"

namespace pio
{
	const int32_t ImGuiUtils::k_FlagTreeNode = ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	const int32_t ImGuiUtils::k_FlagTreeLeaf = ImGuiTreeNodeFlags_Leaf |
		ImGuiTreeNodeFlags_NoTreePushOnOpen;

	const int32_t ImGuiUtils::k_FlagOpenCollapseHeader = ImGuiTreeNodeFlags_DefaultOpen |
		ImGuiTreeNodeFlags_OpenOnDoubleClick |
		ImGuiTreeNodeFlags_OpenOnArrow |
		ImGuiTreeNodeFlags_SpanAvailWidth;

	void ImGuiUtils::DrawImage(int32_t texId, const glm::vec2& imgSize, 
							   const glm::vec2& ltTexCoord, const glm::vec2& rbTexCoord, 
							   float rowWidth, float indent)
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
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
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

	bool ImGuiUtils::DrawImageButton(const char* strId, int32_t texId, const glm::vec2& imgSize, const glm::vec2& ltTexCoord, const glm::vec2& rbTexCoord, const glm::vec4& bgColor, const glm::vec4& tintColor, const char* hover)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
		ImTextureID id = (int32_t*)texId;
		bool click = ImGui::ImageButton(strId, id, ImVec2(imgSize.x, imgSize.y),
										ImVec2(ltTexCoord.x, ltTexCoord.y),
										ImVec2(rbTexCoord.x, rbTexCoord.y),
										ImVec4(bgColor.r, bgColor.g, bgColor.b, bgColor.a),
										ImVec4(tintColor.r, tintColor.g, tintColor.b, tintColor.a));
		ImGui::PopStyleVar();
		if(hover)
		{
			if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort | ImGuiHoveredFlags_NoSharedDelay))
				ImGui::SetTooltip(hover, ImGui::GetStyle().HoverDelayShort);
		}
		return click;
	}

	bool ImGuiUtils::DrawImageModal(const char* strId, int32_t texId, const glm::vec2& ltTexCoord, const glm::vec2& rbTexCoord)
	{
		bool bOpen{ true };
		auto size = ImGui::GetMainViewport()->Size;
		float w = std::min(size.x, size.y) * 0.9f;
		float h = w;
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::OpenPopup(strId);
		ImGui::SetNextWindowSize(ImVec2(w, h));
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if(ImGui::BeginPopupModal(strId, &bOpen, ImGuiWindowFlags_None))
		{
			ImGuiUtils::DrawImage(texId, glm::vec2(w, h));
			ImGui::EndPopup();
		}
		return bOpen;
	}

	void ImGuiUtils::DrawTextWrapped(const std::string& text, float width, float height)
	{
		auto textSize = ImGui::CalcTextSize(text.c_str(), NULL, false);
		if(textSize.x < width)
		{
			ImGui::Text(text.c_str());
		}
		else
		{
			auto lineHeight = ImGui::GetTextLineHeight();
			if(height < lineHeight)
			{
				LOGE("input height[%f] < line height[%f]", height, lineHeight);
				return;
			}
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + width);
			float lineNum = floorf(height / lineHeight);
			float total = lineNum * width;
			if(total > textSize.x || Math::Equal(total, textSize.x))
			{
				ImGui::TextWrapped(text.c_str());
			}
			else
			{
				float diff = textSize.x - total;
				std::string realText;
				float len;
				int32_t idx = text.size() - 1;
				for(; idx >= 0; idx--)
				{
					realText = text.substr(idx);
					len = ImGui::CalcTextSize(realText.c_str(), NULL, false).x;
					if(len > diff || Math::Equal(len, diff))
					{
						break;
					}
				}
				realText = text.substr(0, idx);
				ImGui::TextWrapped(realText.c_str());
			}
			ImGui::PopTextWrapPos();
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