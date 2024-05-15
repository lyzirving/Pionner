#ifndef __PIONNER_UI_IMGUI_UTILS_H__
#define __PIONNER_UI_IMGUI_UTILS_H__

#include "UiDef.h"

namespace pio
{
	class ImGuiUtils
	{
	public:
		static const int32_t Flag_Collapse_Header;
		static const int32_t Flag_Selected_TreeNode;
		static const int32_t Flag_TreeLeaf;

		/*
		* @brief: draw an image using ImGui by texture id
		* @param texId:    texture id
		* @param imgSize:  image's width and height
		* @param rowWidth: row width that contains the item
		* @param indent:   item's horizon offset from row's start
		*/
		static void DrawImage(int32_t texId, const glm::vec2 &imgSize, float rowWidth = 0.f, float indent = 0.f);
		static void HelpMaker(const char *msg);
		static bool bItemClicked();
	private:
		ImGuiUtils() {}
		~ImGuiUtils() = default;
	};
}

#endif