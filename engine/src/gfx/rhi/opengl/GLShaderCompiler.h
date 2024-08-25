#ifndef __PIONNER_GFX_RHI_OPENGL_GL_SHADER_COMPILER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_SHADER_COMPILER_H__

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
		virtual Ref<Shader> doCompile(Ref<RenderContext>& context) override;

	public:
		static Ref<Shader> DoExecute(Ref<RenderContext>& context, const std::string& path);

	private:
		void parseMetadata();

		bool preprocessGLSL();
		bool preprocessStages(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
		bool preprocessIncluders(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
				
		void expandIncluder(IncludeData &data, std::string &source, std::unordered_map<std::string, bool> &expanded);
		bool initShader(Ref<Shader> &shader);

	private:
		std::string m_path{};
		std::string m_source{}, m_name{};
		ShaderUtils::SourceLang m_lang{ ShaderUtils::SourceLang::NONE };
		std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData> m_stageSource{};
	};
}

#endif