#ifndef __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__
#define __PIONNER_GFX_STRUCT_SHADER_LIBRARY_H__

#include "gfx/rhi/Shader.h"

namespace pio
{
	enum class ShaderType : uint8_t
	{
		LineSegment, Color_Line, TextureQuad, Num
	};

	enum class ShaderProgram : uint8_t
	{
		MaterialPreview, GeometryPass, LightingPass, Sprite, SkyBox, DistLightShadowMap, PtLightShadowMap, 
		IBL_EquirecToCube, IBL_DiffuseCnvl, IBL_BrdfConvl, Num
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