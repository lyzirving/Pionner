#ifndef __PIONNER_PLATFORM_OPENGL_GL_SHADER_COMPILER_H__
#define __PIONNER_PLATFORM_OPENGL_GL_SHADER_COMPILER_H__

#include "gfx/rhi/ShaderCompiler.h"

namespace pio
{
	class GLShaderCompiler : public ShaderCompiler
	{
	public:
		GLShaderCompiler(const Ref<Shader> &shader);
		~GLShaderCompiler() = default;

		virtual bool preprocess() override;

	private:
		Ref<Shader> m_shader;
		std::string m_shaderSource;
	};
}

#endif