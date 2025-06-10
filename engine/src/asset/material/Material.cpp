#include "Material.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/ShaderCompiler.h"
#include "gfx/rhi/UniformBuffer.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Material"

namespace pio
{
	Material::Material(const ImportParams& params, ShaderSpecifier spec) : Asset(params), m_Spec(spec)
	{
	}

	void Material::Init()
	{
		if(m_UniformBlock)
			return;

		BuildShader();
		BuildUnformBuffer();

		OnInit();
	}

	void Material::OnAttrsChange()
	{		
		if(!m_UniformBlock || !m_UniformBuffer)
			return;

		if(m_Context.expired())
		{
			LOGE("err! context is invalid[%s]", m_Params.Name.c_str());
			return;
		}

		MakeShaderMutant();

		if(m_UniformBlock->AnyChange())
		{
			m_Context.lock()->UploadData(m_UniformBlock->GetBuffer()->As<void*>(), m_UniformBlock->GetByteUsed(), m_UniformBuffer);
			m_UniformBlock->Invalidate(false);
		}		
	}

	bool Material::Bind(const Ref<Shader>& shader)
	{
		if(!shader || !shader->IsInit())
		{
			LOGE("material[%s]'s shader is invalid", m_Params.Name.c_str());
			return false;
		}

		if(!shader->IsBind())
		{
			LOGE("material[%s]'s shader has not been bound", m_Params.Name.c_str());
			return false;
		}

		if(!m_UniformBuffer)
		{
			LOGE("material[%s]'s ubo is invalid", m_Params.Name.c_str());
			return false;
		}

		return m_UniformBuffer->BindBlock(m_Context.lock(), shader);
	}	

	bool Material::UnBind()
	{
		if(m_UniformBuffer)
		{
			m_UniformBuffer->UnBind();
			return true;
		}
		return false;
	}

	bool Material::NeedCompileMutant() const
	{
		if(m_PermutationFlags == 0)
			return false;

		if(!m_Shader || !m_Shader->Is<ShaderMutant>())
			return true;

		return m_PermutationFlags != m_Shader->As<ShaderMutant>()->GetFlag();
	}

	void Material::BuildShader()
	{
		m_Shader = m_Context.lock()->FindShader(m_Spec);
		MakeShaderMutant();
	}

	void Material::MakeShaderMutant()
	{
		if(NeedCompileMutant())
		{
			m_Shader = m_Shader->MakeMutant(m_PermutationFlags);
		}
	}
}