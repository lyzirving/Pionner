#pragma once

#include "gfx/rhi/ShaderCompiler.h"

namespace pio
{
	class GLShaderCompiler : public ShaderCompiler
	{
		RTTR_ENABLE(ShaderCompiler)
	public:
		GLShaderCompiler();
		~GLShaderCompiler() = default;

	protected:
		virtual void ParseMetadata() override;
		virtual bool Preprocess(ShaderBuilder& builder) override;
		virtual Ref<Shader> CreateShader(const Ref<RenderContext>& context, const ShaderBuilder& builder) override;

	private:		
		bool PreprocessGLSL(ShaderBuilder& builder);
		bool PreprocessStages(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource, ShaderBuilder& builder);
		bool PreprocessIncluders(std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
				
		void ExpandIncluder(IncludeData &data, std::string &source, std::unordered_map<std::string, bool> &expanded);
	};
}