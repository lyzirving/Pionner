#ifndef __PIONNER_SCENE_LIGHT_H__
#define __PIONNER_SCENE_LIGHT_H__

#include "SceneDef.h"
#include "gfx/rhi/Uniform.h"

namespace pio
{
	struct DirectionalLight
	{
		PIO_UNIFORM_STRUCT_DECLARE(DirectionalLight)

		glm::vec3 Direction{ 0.f };
		glm::vec3 Radiance{ 0.f };
		float Intensity{ 0.f };
		float Bias{ 0.0005f };
		int SdMode{ ShadowMode_Soft };
		bool CastShadow{ true };

		// ----------- Non Uniform data -----------
		glm::vec3 Position{ 0.f };
		glm::vec3 Dest{ 0.f };		
		// ----------------------------------------

		DirectionalLight() {}
		DirectionalLight(const glm::vec3 &in_position, const glm::vec3 &in_dest, const glm::vec3 &in_radiance, float in_intensity) 
			: Position(in_position), Dest(in_dest), Radiance(in_radiance), Intensity(in_intensity) 
		{
			calc();
		}

		void calc() { Direction = glm::normalize(Dest - Position); }
	};

	struct DirectionalLightShadowData
	{
		PIO_UNIFORM_STRUCT_DECLARE(DirectionalLightShadowData)

		glm::mat4 ViewMat{1.f};
		glm::mat4 PrjMat{ 1.f };		

		DirectionalLightShadowData() {}
	};

	struct PointLight
	{
		PIO_UNIFORM_STRUCT_DECLARE(PointLight)

		glm::vec3 Position{ 0.f };
		glm::vec3 Radiance{ 0.f };
		float Intensity{ 0.f };
		float MinRadius{ 0.f };
		float Radius{ 0.f };
		float Falloff{ 0.f };
		float SourceSize{ 0.f };

		// ----------- Non Uniform data -----------
		AssetHandle Volume{ NullAsset };
		// ----------------------------------------

		PointLight() {}
		PointLight(const glm::vec3 &pos, const glm::vec3 &radiance, float intensity,
				   float minRadius, float radius, float falloff, float srcSize)
			: Position(pos), Radiance(radiance), Intensity(intensity), MinRadius(minRadius),
			  Radius(radius), Falloff(falloff), SourceSize(srcSize)
		{
		}
	};

	struct SpotLight
	{
		// TODO:
	};

	struct PointLightData
	{
		PIO_UNIFORM_STRUCT_DECLARE(PointLightData)

		int32_t LightCount{ 0 };
		PointLight Lights[POINT_LIGHT_LIMIT];

		PointLight &operator[](uint32_t i) { return Lights[i]; }
	};

	struct PointLightMetadata
	{
		glm::mat4 LightSpaceMat[6];
		glm::vec3 Position;
		float FrustumFar;
	};

	struct PointLightShadowData
	{
		PIO_UNIFORM_STRUCT_DECLARE(PointLightShadowData)

		int32_t LightCount;
		PointLightMetadata Metadata[POINT_LIGHT_LIMIT];
	};

	struct LightEnvironment
	{
		DirectionalLight DirectionalLight;
		DirectionalLightShadowData DirectionalLightShadowData;
		PointLightData PointLightData;
		PointLightShadowData PtLightShadowData;
	};
}

#endif