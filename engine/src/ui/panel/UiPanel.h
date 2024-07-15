#ifndef __PIONNER_UI_PANEL_UI_PANEL_H__
#define __PIONNER_UI_PANEL_UI_PANEL_H__

#include "ui/UiDef.h"

namespace pio
{
	class Entity;
	class Transform;

	class UiPanel
	{
	public:
		static void DrawNamePanel(const char *nLabel, const std::string &name,
							      const char* vLabel, bool &visible, 
								  uint32_t rowWidth);
		static void DrawTransformPanel(Transform& transform);
		static void DrawTransformPanel(Ref<Entity> &entity);
	};
}

#endif