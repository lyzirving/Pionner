#include "ShaderLab.h"
#include "RenderContext.h"
#include "ShaderUtils.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/ShaderCompiler.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderLab"

namespace pio
{
	ShaderLab::ShaderLab(const Ref<RenderContext>& context)
	{
		m_Context = context;
	}

	ShaderLab::~ShaderLab() = default;

	Ref<Shader> ShaderLab::Get(ShaderSpecifier spec)
	{
		auto it = m_Shaders.find(spec);
		if(it != m_Shaders.end())
		{
			return it->second;
		}
		ShaderBuilder builder;
		builder.SetName(ShaderUtils::GetName(spec))
			.SetPath(Path::ShaderRoot());
		auto shader = ShaderCompiler::Compile(m_Context.lock(), builder);
		if(!shader)
		{
			LOGE("fail to compile shader[%s]", ShaderUtils::GetName(spec));
			std::abort();
		}
		shader->m_Builder = builder;
		m_Shaders.insert({ spec, shader });
		return shader;
	}

	void ShaderLab::Release()
	{
		auto it = m_Shaders.begin();
		while(it != m_Shaders.end())
		{
			it = m_Shaders.erase(it);
		}
	}
}