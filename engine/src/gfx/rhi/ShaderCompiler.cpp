#include "ShaderCompiler.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderCompiler"

namespace pio
{

	Ref<ShaderCompiler> ShaderCompiler::Create(const Ref<Shader>& shader)
	{
		switch (RenderAPI::CurrentType())
		{
		case RenderAPI::Type::OpenGL:
			return CreateRef<GLShaderCompiler>(shader);
		default:
			return Ref<ShaderCompiler>();
		}
	}

}