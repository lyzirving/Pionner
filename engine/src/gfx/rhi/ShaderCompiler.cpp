#include "ShaderCompiler.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderCompiler"

namespace pio
{
	Ref<Shader> ShaderCompiler::Compile(Ref<RenderContext>& context, const std::string& path)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
		{
			return GLShaderCompiler::DoExecute(context, path);
		}
		default:
			LOGE("err! render backend[%u] has not been implemented", context->renderBackend());
			return Ref<Shader>();			
		}		
	}
}