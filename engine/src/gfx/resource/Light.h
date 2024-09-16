#ifndef __PIONNER_GFX_RESOURCE_LIGHT_H__
#define __PIONNER_GFX_RESOURCE_LIGHT_H__

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
}

#endif