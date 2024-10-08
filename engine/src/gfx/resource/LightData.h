#ifndef __PIONNER_GFX_RESOURCE_LIGHT_DATA_H__
#define __PIONNER_GFX_RESOURCE_LIGHT_DATA_H__

#include "gfx/rhi/UniformBuffer.h"

namespace pio
{
	struct DirectionalLightUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(DirectionalLightUD)

		glm::vec3 Direction{ 0.f };		
		glm::vec3 Color{ 0.f };//Light color, each component should be in [0, 1]
		float Intensity{ 0.f };
		float Bias{ 0.05f };
		float NormalBias{ 0.4 };
		int ShadowMode{ ShadowMode_Soft };
		bool CastShadow{ true };
		float ShadowIntensity{ 1.f };
	};

	struct DirectionalLightShadowDataUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(DirectionalLightShadowDataUD)

		glm::mat4 ViewMat{ 1.f };
		glm::mat4 PrjMat{ 1.f };
		glm::vec2 ShadowMapSize{ 0.f };
		float FrustumSize{ 0.f };
	};

	struct CascadeShadowMapUD
	{
		PIO_UNIFORM_STRUCT_DECLARE(CascadeShadowMapUD)
		
		float ClipSpaceEnd[CASCADE_NUM];
		glm::vec4 IndicateColor[CASCADE_NUM];//Colors for debug CSM
		glm::mat4 PrjMats[CASCADE_NUM];
		glm::mat4 ViewMat{ 1.f };
		int32_t CascadeNum{ CASCADE_NUM };
	};
}

#endif