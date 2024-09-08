#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include "rhi/RhiDef.h"

namespace pio
{
	namespace GpuAttr
	{
		//Default texture
		static const char* WHITE_TEXTURE = "WhiteTexture";
		static const char* BLACK_TEXTURE = "BlackTexture";
		//Uniform data
		static const char* UNI_MODEL_MAT = "u_modelMat";
		//Uniform block
		static const char* BINDING_CAM_BLOCK = "CameraBlock";

		//GBuffer in Deferred pass
		static const char* UNI_GBUFFER_POS      = "u_GPosition";
		static const char* UNI_GBUFFER_NORMAL   = "u_GNormal";
		static const char* UNI_GBUFFER_ALBEDO   = "u_GAlbedoAlpha";
		static const char* UNI_GBUFFER_MATERIAL = "u_GMaterial";
		static const char* UNI_GBUFFER_EMISSION = "u_GEmission";
	}

	enum RenderingMode : uint8_t
	{
		RenderingMode_Opaque = 0,
		RenderingMode_Transparent,
		RenderingMode_Num
	};

	enum class RenderingStrategy : uint8_t
	{
		Forward = 0, Deffered
	};

	struct RenderConfig
	{
		RenderingStrategy Strategy{ RenderingStrategy::Deffered };
		glm::vec4 ClearColor{ 0.f, 0.f, 0.f, 1.f };
	};
}

#endif