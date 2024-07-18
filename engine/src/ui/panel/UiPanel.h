#ifndef __PIONNER_UI_PANEL_UI_PANEL_H__
#define __PIONNER_UI_PANEL_UI_PANEL_H__

#include "ui/UiDef.h"

namespace pio
{
	class Entity;
	class Transform;

	enum DataAttrBits : uint8_t
	{
		DataAttrBits_Pos = 0, DataAttrBits_Rot, DataAttrBits_Scale,
		DataAttrBits_Num
	};

	using DataAttrs = std::bitset<DataAttrBits_Num>;

	class UiPanel
	{
	public:
		static void DrawNamePanel(const char *nLabel, const std::string &name,
							      const char* vLabel, bool &visible, 
								  uint32_t rowWidth);
		static DataAttrs DrawTransformPanel(Transform& transform);
		static DataAttrs DrawTransformPanel(Ref<Entity> &entity);
	};
}

#endif