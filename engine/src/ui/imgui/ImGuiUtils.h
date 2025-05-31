#pragma once

#include "ui/UiDef.h"

namespace pio
{
	class ImGuiUtils
	{
	public:
		/*
		* @brief: draw an image using ImGui by texture id
		* @param texId: texture id
		* @param imgSize: image's width and height
		* @param ltTexCoord	texture coord of the left-top point
		* @param rbTexCoord	texture coord of the right-bottom point
		* @param rowWidth: row width that contains the item
		* @param indent: item's horizon offset from row's start
		*/
		static void DrawImage(int32_t texId, const glm::vec2& imgSize, 
							  const glm::vec2& ltTexCoord = glm::vec2(0.f), 
							  const glm::vec2& rbTexCoord = glm::vec2(1.f), 
							  float rowWidth = 0.f, float indent = 0.f);
		static bool DrawImageButton(const char* strId, int32_t texId, const glm::vec2& imgSize,
									const glm::vec2& ltTexCoord = glm::vec2(0.f),
									const glm::vec2& rbTexCoord = glm::vec2(1.f),
									const glm::vec4& bgColor = glm::vec4(0.f, 0.f, 0.f, 1.f),
									const glm::vec4& tintColor = glm::vec4(1.f),
									const char* hover = nullptr);
		static bool DrawImageModal(const char* strId, int32_t texId,
								   const glm::vec2& ltTexCoord = glm::vec2(0.f),
								   const glm::vec2& rbTexCoord = glm::vec2(1.f));
		static void DrawTextWrapped(const std::string& text, float width, float height);
		static void HelpMaker(const char* msg);
		static bool ItemBeingClicked();

	public:
		static const int32_t k_FlagTreeNode;
		static const int32_t k_FlagTreeLeaf;
		static const int32_t k_FlagOpenCollapseHeader;
	};
}