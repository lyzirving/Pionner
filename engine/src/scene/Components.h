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
		UUID32 Hnd{ InvalidId };
	};

	struct MeshFilter : public Component
	{
		MeshType Type{ MeshType::Num };
		UUID32 MeshHnd{ InvalidId };
	};

	struct MeshRenderer : public Component
	{
		UUID32 MatHnd{ InvalidId }; //Uid for material asset
		UUID32 BuffHnd{ InvalidId };//Uid for MeshRenderBuffer
	};

	struct SpriteRenderer : public Component
	{
		glm::vec3 Color{ 1.f };
		bool FlipX{ false };
		bool FlipY{ false };
		bool BillBoard{ false };
		UUID32 MatHnd{ InvalidId }; 
		UUID32 MeshHnd{ InvalidId };
		UUID32 BuffHnd{ InvalidId };
	};

	struct DirectionalLightComponent : public Component
	{
		UUID32 BuffId{ InvalidId };
		uint32_t Type{ (uint32_t)LightType::DirectionLight };
		glm::vec3 Color{ 0.6f, 0.6f, 0.6f };//Each component of color should be clamped into [0, 1]
		float Intensity{ 1.0f };
		// ----- Shadow related -------
		float Bias{ 0.003f };
		float NormalBias{ 0.4f };
		bool CastShadow{ true };
		int ShadowMode{ ShadowMode_Soft };		
		float ShadowIntensity{ 1.f };
		// ----------------------------
	};
}

#endif