#pragma once

#include "asset/Asset.h"
#include "gfx/GfxDef.h"
#include "gfx/renderer/ShaderUtils.h"

namespace pio
{
	class Shader;
	class UniformBlock;
	class UniformBuffer;	

	class Material : public Asset
	{
		RTTR_ENABLE(Asset)
	public:
		Material(const ImportParams& params, ShaderSpecifier spec);
		virtual ~Material() = default;
		virtual void Init() override;
		virtual bool IsInit() const override { return m_UniformBuffer.use_count() != 0; }
		virtual void OnAttrsChange() override;

		virtual bool Bind(const Ref<Shader>& shader);				
		virtual bool UnBind();

		virtual bool IsTranslucency() const = 0;

		void SetShadowCastMode(ShadowCastMode mode) { m_ShadowCastMode = mode; }
		bool IsCastShadow() const
		{
			return m_ShadowCastMode != ShadowCastMode_Off && m_ShadowCastMode < ShadowCastMode_Num;
		}

		Ref<Shader>& GetShader() { return m_Shader; }
		const Ref<Shader>& GetShader() const { return m_Shader; }
		const ShaderSpecifier GetShaderSpec() const { return m_Spec; }
		ShadowCastMode GetShadowCastMode() const { return m_ShadowCastMode; }
		const Ref<UniformBuffer>& GetUBuffer() const { return m_UniformBuffer; }
		
		bool HasFlag(ShaderPermutationFlag flag) const { return m_PermutationFlags & ((uint32_t)flag); }
		void AddFlag(ShaderPermutationFlag flag) { m_PermutationFlags |= (uint32_t)flag; }
		void RemoveFlag(ShaderPermutationFlag flag) { m_PermutationFlags &= ~((uint32_t)flag); }
		void ClearFlag() { m_PermutationFlags = 0; }

	protected:
		virtual void BuildShader();
		virtual void BuildUnformBuffer() = 0;

		bool NeedCompileMutant() const;
		void MakeShaderMutant();

	protected:		
		Ref<UniformBlock> m_UniformBlock;
		Ref<UniformBuffer> m_UniformBuffer;
		const ShaderSpecifier m_Spec;

		uint32_t m_PermutationFlags{ 0 };
		ShadowCastMode m_ShadowCastMode{ ShadowCastMode_On };
		mutable Ref<Shader> m_Shader;		
	};

}