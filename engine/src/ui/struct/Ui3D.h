#ifndef __PIONNER_UI_STRUCT_UI3D_H__
#define __PIONNER_UI_STRUCT_UI3D_H__

#include "ui/UiDef.h"

namespace pio
{
	class Entity;

	struct CylinderConfig
	{
		uint32_t Itr{ 36 };
		float Radius{ 0.05f };
		float Height{ 0.8f };
	};

	struct ArrowConfig
	{
		uint32_t Itr{ 36 };
		float CylinderRadius{ 0.013f };
		float CylinderHeight{ 0.4f };
		float ConeRadius{ 0.025f };
		float ConeHeight{ 0.1f };
		float Offset{0.06f};
	};

	struct UiCoordinate3DBuilder
	{
		CoordinateShape Shape;
		CylinderConfig CylinderInfo;
		ArrowConfig ArrowInfo;
	};

	class UiCoordinate3D
	{
	public:
		UiCoordinate3D(const UiCoordinate3DBuilder &builder);
		~UiCoordinate3D() = default;

	private:
		void build(const UiCoordinate3DBuilder &builder);
		void buildImpl(const CylinderConfig &config);
		void buildImpl(const ArrowConfig &config);

	public:
		CoordinateShape Shape;
		UiCoordinate3DBuilder Builder;
		Ref<Entity> XAxisEnt, YAxisEnt, ZAxisEnt;
	};
}

#endif