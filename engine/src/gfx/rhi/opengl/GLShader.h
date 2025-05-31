#pragma once

#include "gfx/rhi/Shader.h"

namespace pio
{
	class GLShader : public Shader
	{
		RTTR_ENABLE(Shader)
	public:
		GLShader(const Ref<RenderContext>& context, const std::string& name,
				 const std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
		~GLShader() = default;

		virtual void SetBool(const std::string& name, bool val) override;
		virtual void SetFloat(const std::string &name, float val) override;
		virtual void SetUInt(const std::string &name, uint32_t val) override;
		virtual void SetInt(const std::string &name, int32_t val) override;

		virtual void SetMat2(const std::string& name, const glm::mat2& mat) override;
		virtual void SetMat3(const std::string &name, const glm::mat3 &mat) override;
		virtual void SetMat4(const std::string &name, const glm::mat4 &mat) override;

		virtual void SetVec2(const std::string &name, const glm::vec2 &vec2) override;
		virtual void SetVec3(const std::string &name, const glm::vec3 &vec3) override;
		virtual void SetVec4(const std::string &name, const glm::vec4 &vec4) override;

		virtual void SetIVec2(const std::string &name, const glm::ivec2 &vec2) override;
		virtual void SetIVec3(const std::string &name, const glm::ivec3 &vec3) override;
		virtual void SetIVec4(const std::string &name, const glm::ivec4 &vec4) override;

		virtual void SetTextureSampler(const std::string& name, TextureSampler slot) override;
		virtual void FreeSamplerUnit() override { m_OccupiedSlots.reset(); }

		virtual bool Init() override;
		virtual bool IsInit() const override { return m_ID > 0; }
		virtual void Destroy() override;

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual bool GetSampler(TextureSampler& out) const override;	

	private:
		std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData> m_StageSource{};
		std::bitset<PIO_UINT8(TextureSampler::MaxSlotNum)> m_OccupiedSlots;

	private:
		friend class GLShaderCompiler;
	};

	class GLShaderMutant : public ShaderMutant
	{
		RTTR_ENABLE(ShaderMutant)
	public:
		GLShaderMutant(const Ref<RenderContext>& context, const std::string& name,
					   const std::map<ShaderUtils::ShaderStageFlagBits, ShaderUtils::StageData>& stageSource);
		~GLShaderMutant() = default;

		virtual void SetBool(const std::string& name, bool val) override { m_Shader->SetBool(name, val); }
		virtual void SetFloat(const std::string& name, float val) override { m_Shader->SetFloat(name, val); }
		virtual void SetUInt(const std::string& name, uint32_t val) override { m_Shader->SetUInt(name, val); }
		virtual void SetInt(const std::string& name, int32_t val) override { m_Shader->SetInt(name, val); }

		virtual void SetMat2(const std::string& name, const glm::mat2& mat) override { m_Shader->SetMat2(name, mat); }
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) override { m_Shader->SetMat3(name, mat); }
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) override { m_Shader->SetMat4(name, mat); }

		virtual void SetVec2(const std::string& name, const glm::vec2& vec2) override { m_Shader->SetVec2(name, vec2); }
		virtual void SetVec3(const std::string& name, const glm::vec3& vec3) override { m_Shader->SetVec3(name, vec3); }
		virtual void SetVec4(const std::string& name, const glm::vec4& vec4) override { m_Shader->SetVec4(name, vec4); }

		virtual void SetIVec2(const std::string& name, const glm::ivec2& vec2) override { m_Shader->SetIVec2(name, vec2); }
		virtual void SetIVec3(const std::string& name, const glm::ivec3& vec3) override { m_Shader->SetIVec3(name, vec3); }
		virtual void SetIVec4(const std::string& name, const glm::ivec4& vec4) override { m_Shader->SetIVec4(name, vec4); }

		virtual void SetTextureSampler(const std::string& name, TextureSampler slot) override { m_Shader->SetTextureSampler(name, slot); }
		virtual void FreeSamplerUnit() override { m_Shader->FreeSamplerUnit(); }

		virtual bool Init() override { return m_Shader->Init(); }
		virtual bool IsInit() const override { return m_Shader->IsInit(); }
		virtual void Destroy() override { m_Shader->Destroy(); }

		virtual void Bind() override { m_Shader->Bind(); }
		virtual void UnBind() override { m_Shader->UnBind(); }
		virtual bool IsBind() const override { return m_Shader->IsBind(); }

		virtual uint32_t Id() const { return m_Shader->Id(); }

		virtual bool GetSampler(TextureSampler& out) const override { return m_Shader->GetSampler(out); }

	private:
		Ref<GLShader> m_Shader;

	private:
		friend class GLShaderCompiler;
	};
}