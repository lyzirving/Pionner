#ifndef __PIONNER_GFX_RENDERER_RESOURCE_RENDER_SHADER_H__
#define __PIONNER_GFX_RENDERER_RESOURCE_RENDER_SHADER_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Shader;
	class RenderContext;

	class RenderShader
	{
	public:
		RenderShader(ShaderSpecifier spec) : m_spec(spec) {}
		virtual ~RenderShader() = default;

		bool compile(Ref<RenderContext> &context, const std::string &path);

		Ref<Shader> impl() const { return m_shader; }
		ShaderSpecifier spec() const { return m_spec; }

		bool valid() const { return m_shader.use_count() != 0; }

	protected:
		Ref<Shader> m_shader;
		ShaderSpecifier m_spec{ ShaderSpec_Num };
	};
}

#endif