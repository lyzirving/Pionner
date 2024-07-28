#ifndef __PIONNER_UI_PANEL_UI_PANEL_H__
#define __PIONNER_UI_PANEL_UI_PANEL_H__

#include "ui/UiDef.h"

namespace pio
{
	class Entity;
	class Transform;
	struct CameraComponent;

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
								  uint32_t rowWidth = 0);
		static DataAttrs DrawTransformPanel(Transform& transform);
		static DataAttrs DrawTransformPanel(Ref<Entity> &entity);
		static void DrawLightPanel(Ref<Entity>& entity);
		static void DrawCameraPanel(CameraComponent &comp);

	private:
		static void DrawDirectionalLightPanel(Ref<Entity>& entity);
		static void DrawPointLightPanel(Ref<Entity>& entity);
	};
}

#endif