#ifndef __PIONNER_UI_IMGUI_UTILS_H__
#define __PIONNER_UI_IMGUI_UTILS_H__

#include "UiDef.h"
#include <imgui.h>

namespace pio
{
	class ImGuiUtils
	{
	public:
		static const int32_t k_FlagCollapseHeader;
		static const int32_t k_FlagSelectedNode;
		static const int32_t k_FlagTreeLeaf;
		static const int32_t k_FlagCommonWindow;

		/*
		* @brief: draw an image using ImGui by texture id
		* @param texId:		texture id
		* @param imgSize:	image's width and height
		* @param ltTexCoord	texture coord of the left-top point
		* @param rbTexCoord	texture coord of the right-bottom point
		* @param rowWidth:	row width that contains the item
		* @param indent:	item's horizon offset from row's start
		*/
		static void DrawImage(int32_t texId, const glm::vec2& imgSize, const glm::vec2& ltTexCoord = glm::vec2(0.f), const glm::vec2& rbTexCoord = glm::vec2(1.f), float rowWidth = 0.f, float indent = 0.f);
		static void HelpMaker(const char* msg);
		static bool ItemBeingClicked();

	private:
		ImGuiUtils() {}
		~ImGuiUtils() = default;
	};
}

#endif