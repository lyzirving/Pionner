#ifndef __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__
#define __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__

#include "gfx/rhi/Shader.h"

namespace pio
{
	enum class ShaderType : uint8_t
	{
		PBR_Mesh, DistantLight_ShadowData, PointLight_ShadowData, MaterialPreview, Wireframe, LightVolume,
		LineSegment, Color_Line, Outline, TextureQuad, Sprite,
		Outline_Deferred,
		Equirectangular_To_Cube, Diffuse_Convolution, PrefilterMap_Convolution, Brdf_Convolution, 
		Skybox, Postprocessing, Num
	};

	enum class ShaderProgram : uint8_t
	{
		GeometryPass, LightingPass, Num
	};

	class ShaderLibrary
	{
		PIO_SINGLETON_DECLARE(ShaderLibrary)
	public:
		static const char* ToStr(ShaderProgram type);

	public:
		Ref<Shader> find(ShaderType type);
		
		void add(ShaderProgram type, const Ref<Shader>& shader) { m_shaderPrograms[PIO_UINT8(type)] = shader; }
		Ref<Shader> find(ShaderProgram type)
		{
			Ref<Shader> shader = m_shaderPrograms[PIO_UINT8(type)];
			assert(shader.get(), "shader[%s] is invalid", ToStr(type));
			return shader;
		}
		void release();

	private:
		Ref<Shader> m_shaders[PIO_UINT(ShaderType::Num)];
		Ref<Shader> m_shaderPrograms[PIO_UINT(ShaderProgram::Num)];
	};
}

#endif