#ifndef __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__
#define __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__

#include "gfx/rhi/Shader.h"

namespace pio
{
	enum class ShaderType : uint8_t
	{
		PBR_Mesh, DistantLight_ShadowData, PointLight_ShadowData, MaterialPreview, Wireframe, LightVolume,
		Color_Line, Outline, TextureQuad, Sprite,
		Mesh_Deferred, DistantLighting_Deferred, PointLightEffect_Deferred, Outline_Deferred,
		Equirectangular_To_Cube, Diffuse_Convolution, Skybox, Postprocessing, Num
	};

	class ShaderLibrary
	{
		PIO_SINGLETON_DECLARE(ShaderLibrary)

	public:
		Ref<Shader> find(ShaderType type);
		void release();

	private:
		Ref<Shader> m_shaders[PIO_UINT(ShaderType::Num)];
	};
}

#endif