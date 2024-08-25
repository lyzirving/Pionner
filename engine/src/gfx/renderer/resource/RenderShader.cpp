#include "RenderShader.h"

#include "gfx/rhi/ShaderCompiler.h"

namespace pio
{
	bool RenderShader::compile(Ref<RenderContext>& context, const std::string& path)
	{
		m_shader = ShaderCompiler::Compile(context, path);
		return valid();
	}
}