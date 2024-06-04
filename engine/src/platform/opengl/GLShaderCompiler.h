#ifndef __PIONNER_PLATFORM_OPENGL_GL_SHADER_COMPILER_H__
#define __PIONNER_PLATFORM_OPENGL_GL_SHADER_COMPILER_H__

#include "gfx/rhi/ShaderCompiler.h"

namespace pio
{
	class GLShaderCompiler : public ShaderCompiler
	{
	public:
		GLShaderCompiler();
		~GLShaderCompiler() = default;

	protected:
		virtual bool preprocess() override;

	public:
		static Ref<Shader> DoCompile(const std::string& path);

	private:
		void parseMetadata();

		bool preprocessGLSL();
		bool preprocessShader(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
		bool preprocessIncluders(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
				
		void expandIncluder(const IncludeData &data, std::string &source, std::unordered_map<std::string, bool> &expanded);

	private:
		std::string m_path{};
		std::string m_source{}, m_name{};
		ShaderUtils::SourceLang m_lang{ ShaderUtils::SourceLang::NONE };
	};
}

#endif