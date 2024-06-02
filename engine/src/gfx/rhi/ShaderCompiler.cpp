#include "ShaderCompiler.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderCompiler"

namespace pio
{
	Ref<Shader> ShaderCompiler::Compile(const std::string& path)
	{
		switch (RenderAPI::CurrentType())
		{
		case RenderAPI::Type::OpenGL:
			return GLShaderCompiler::DoCompile(path);
		default:
			return Ref<Shader>();
		}
	}

}