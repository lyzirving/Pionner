#ifndef __PIONNER_UI_UI3D_H__
#define __PIONNER_UI_UI3D_H__

#include "UiDef.h"

namespace pio
{
	class Entity;
	class LineMesh;

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

	class UiRotationCtl
	{
	public:
		UiRotationCtl();
		~UiRotationCtl();

	private:
		void build();

	public:
		Ref<Entity> XTorus, YTorus, ZTorus;
	};

	class UiDistantLight
	{
	public:
		UiDistantLight(float radius, float lightLen, const glm::vec4 &color);
		~UiDistantLight() = default;

	private:
		static Ref<LineMesh> CreateLightMesh(float radius, float lightLen, const glm::vec4 &color);
		static Ref<LineMesh> CreateDirectonMesh(const glm::vec4 &color);

	public:
		Ref<Entity> LightMesh;
		Ref<Entity> DirectionMesh;
		glm::vec4 Color{ 0.f, 0.f, 0.f, 1.f };
		const float DirectionLen{ 20.f };
	};
}

#endif