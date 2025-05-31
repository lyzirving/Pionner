#pragma once

#include "RenderResource.h"
#include "gfx/renderer/ShaderUtils.h"

namespace pio
{
	class UniformData;
	class ShaderMutant;

	class Shader : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		Shader(const Ref<RenderContext>& context, const std::string& name) : RenderResource(context, RenderResourceType::Shader, name) {}
		virtual ~Shader() = default;

		virtual bool GetSampler(TextureSampler& out) const = 0;

		virtual void SetBool(const std::string& name, bool val) = 0;
		virtual void SetFloat(const std::string& name, float val) = 0;
		virtual void SetInt(const std::string& name, int32_t val) = 0;
		virtual void SetUInt(const std::string& name, uint32_t val) = 0;

		virtual void SetMat2(const std::string& name, const glm::mat2& mat) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& mat) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;

		virtual void SetVec2(const std::string& name, const glm::vec2& vec2) = 0;
		virtual void SetVec3(const std::string& name, const glm::vec3& vec3) = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& vec4) = 0;

		virtual void SetIVec2(const std::string& name, const glm::ivec2& vec2) = 0;
		virtual void SetIVec3(const std::string& name, const glm::ivec3& vec3) = 0;
		virtual void SetIVec4(const std::string& name, const glm::ivec4& vec4) = 0;

		virtual void SetTextureSampler(const std::string& name, TextureSampler slot) = 0;
		virtual void FreeSamplerUnit() = 0;
	
	public:	
		void SetUniformData(Ref<UniformData>& unimData);
		Ref<ShaderMutant> MakeMutant(uint32_t flags);

	protected:
		Ref<ShaderMutant> FindMutant(uint32_t flags);

	protected:
		ShaderBuilder m_Builder;
		std::unordered_map<uint32_t, Ref<ShaderMutant>> m_Permutations;

		friend class ShaderLab;
	};

	class ShaderMutant : public Shader
	{
		RTTR_ENABLE(Shader)
	public:
		ShaderMutant(const Ref<RenderContext>& context, const std::string& name) : Shader(context, name) {}
		virtual ~ShaderMutant() = default;

		Ref<Shader> GetParent() const { return m_Parent.lock(); }
		uint32_t GetFlag() const { return m_PermutationFlag; }

	protected:
		WeakRef<Shader> m_Parent;
		uint32_t m_PermutationFlag{ 0 };

	private:
		friend class Shader;
	};
}