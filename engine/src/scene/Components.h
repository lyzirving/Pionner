#ifndef __PIONNER_SCENE_COMPONENTS_H__
#define __PIONNER_SCENE_COMPONENTS_H__

#include "Common.h"
#include "gfx/GfxDef.h"

namespace pio
{
	struct Component
	{
		bool Enable{ true };
	};

	struct TransformComponent : public Component
	{
		glm::vec3 Position{ 0.f };		
		glm::vec3 Rotation{ 0.f };//Euler angle respectivly for x/y/z axis
		glm::vec3 Scale{ 1.f };
	};

	struct CameraComponent : public Component
	{
		ProjectionType PrjType{ ProjectionType_Perspective };
		int32_t Depth{ 0 };
		float Near{ 0.1f }, Far{ 30.f };
		float Fov{ 60.f };
		float Aspect{ 1.f };
		float Size{ 5.f };
		UUID32 Uid{ InvalidId };
	};

	struct MeshFilter : public Component
	{
		MeshType Type{ MeshType::Num };
		UUID32 Uid{ InvalidId };
	};

	struct MeshRenderer : public Component
	{
		UUID32 MatUid{ InvalidId }; //Uid for material asset
		UUID32 BuffUid{ InvalidId };//Uid for MeshRenderBuffer
	};

	struct DirectionalLightComponent : public Component
	{
		UUID32 BuffId{ InvalidId };
		uint32_t Type{ (uint32_t)LightType::DirectionLight };
		glm::vec3 Color{ 0.0f, 0.0f, 0.0f };
		float Intensity{ 0.0f };
		// ----- Shadow related -------
		float Bias{ 0.05f };
		float NormalBias{ 0.4f };
		bool CastShadow{ true };
		int ShadowMode{ ShadowMode_Soft };		
		float ShadowIntensity{ 1.f };
		// ----------------------------
	};
}

#endif