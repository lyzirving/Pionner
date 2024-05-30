#include <iostream>
#include <fstream>
#include <sstream>

#include "GLShaderCompiler.h"

#include "gfx/rhi/Shader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLShaderCompiler"

namespace pio
{
	GLShaderCompiler::GLShaderCompiler(const Ref<Shader>& shader) : ShaderCompiler(), m_shader(shader)
	{
		std::ifstream shaderFile;
		std::stringstream ss;

		const std::string& path = m_shader->getPath();
		shaderFile.open(path);
		if (!shaderFile.is_open())
		{
			LOGE("fail to open shader file[%s]", path.c_str());
			return;
		}

		ss << shaderFile.rdbuf();
		shaderFile.close(); 
		m_shaderSource = ss.str();
	}

	bool GLShaderCompiler::preprocess()
	{
		if (m_shaderSource.empty())
		{
			LOGE("invalid shader source for shader[%s]", m_shader->getName().c_str());
			return false;
		}

		return false;
	}
}