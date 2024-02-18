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

		static void HelpMaker(const char *msg);
		static bool IsItemClicked();
	private:
		ImGuiUtils() {}
		~ImGuiUtils() = default;
	};
}

#endif