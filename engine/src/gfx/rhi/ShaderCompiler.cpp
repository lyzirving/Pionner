#include "ShaderCompiler.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/opengl/GLShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderCompiler"

namespace pio
{
	Ref<Shader> ShaderCompiler::Compile(const Ref<RenderContext>& context, ShaderBuilder& builder)
	{
		auto compiler = ShaderCompiler::Create(context);
		compiler->m_Path = builder.GetFullPath();
		compiler->m_Source = StringUtil::ReadFileSource(compiler->m_Path);
		compiler->ParseMetadata();
		LOGD("compiling shader[%s, %s] from[%s]", compiler->m_Name.c_str(),
			 ShaderUtils::LanguageToString(compiler->m_Lang), compiler->m_Path.c_str());

		if (!compiler->Preprocess(builder))
		{
			LOGE("err! compiler[%s] Preprocess fail", compiler->m_Name.c_str());
			return Ref<Shader>();
		}
		auto shader = compiler->CreateShader(context, builder);
		if(context->IsRenderThread())
		{
			shader->Init();
		}
		return shader;
	}

	Ref<ShaderCompiler> ShaderCompiler::Create(const Ref<RenderContext>& context)
	{
		switch (context->RenderBackend())
		{
			case RenderBackend_OpenGL:
			{
				return CreateRef<GLShaderCompiler>();
			}
			default:
				LOGE("err! render backend[%u] has not been implemented", context->RenderBackend());
				return Ref<ShaderCompiler>();
		}
	}
}