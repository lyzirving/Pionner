#include "StandardMaterial.h"
#include "asset/image/Image.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "StandardMaterial"

namespace pio
{
	StandardMaterial::StandardMaterial(const ImportParams& params) : Material(params, SHADER_SPEC_STANDARD)
	{
	}	

	bool StandardMaterial::BindAt(const Ref<Shader>& shader)
	{
		if(!Material::BindAt(shader))
			return false;

		if(Texture2D* t; m_AlbedoMap && (t = m_AlbedoMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(shader, "u_AlbedoMap");
		}

		if(Texture2D* t; m_MetallicRoughness && (t = m_MetallicRoughness->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(shader, "u_MetallicRoughnessMap");
		}

		if(Texture2D* t; m_NormalMap && (t = m_NormalMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(shader, "u_NormalMap");
		}

		if(Texture2D* t; m_EmissionMap && (t = m_EmissionMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(shader, "u_EmissionMap");
		}
		return true;
	}

	bool StandardMaterial::Bind()
	{
		if(!Material::Bind())
			return false;

		if(Texture2D* t; m_AlbedoMap && (t = m_AlbedoMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(m_Shader, "u_AlbedoMap");
		}

		if(Texture2D* t; m_MetallicRoughness && (t = m_MetallicRoughness->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(m_Shader, "u_MetallicRoughnessMap");
		}

		if(Texture2D* t; m_NormalMap && (t = m_NormalMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(m_Shader, "u_NormalMap");
		}

		if(Texture2D* t; m_EmissionMap && (t = m_EmissionMap->GetTexture()->As<Texture2D>()))
		{
			t->BindAt(m_Shader, "u_EmissionMap");
		}

		return true;
	}

	bool StandardMaterial::UnBind()
	{		
		Material::UnBind();

		if(m_AlbedoMap)
			m_AlbedoMap->GetTexture()->As<Texture2D>()->UnBind();

		if(m_MetallicRoughness)
			m_MetallicRoughness->GetTexture()->As<Texture2D>()->UnBind();

		if(m_NormalMap)
			m_NormalMap->GetTexture()->As<Texture2D>()->UnBind();

		if(m_OcclusionMap)
			m_OcclusionMap->GetTexture()->As<Texture2D>()->UnBind();

		if(m_EmissionMap)
			m_EmissionMap->GetTexture()->As<Texture2D>()->UnBind();

		return true;
	}

	Ref<Asset> StandardMaterial::SpawnChild()
	{
		auto material = CreateRef<StandardMaterial>(m_Params);
		material->m_Albedo = m_Albedo;
		material->m_Emission = m_Emission;
		material->m_Metallic = m_Metallic;
		material->m_Roughness = m_Roughness;
		material->m_Occlusion = m_Occlusion;
		material->m_Alpha = m_Alpha;
		material->m_bUseNormalMap = m_bUseNormalMap;

		material->m_AlbedoMap = m_AlbedoMap;
		material->m_MetallicRoughness = m_MetallicRoughness;
		material->m_NormalMap = m_NormalMap;
		material->m_OcclusionMap = m_OcclusionMap;
		material->m_EmissionMap = m_EmissionMap;
		material->Init();
		return material;
	}

	void StandardMaterial::OnInit()
	{	
		OnAlbedoChange();
		OnEmissionChange();
		OnMetallicChange();
		OnRoughnessChange();
		OnOcclusionChange();
		OnAlphaChange();
		OnbUseNormalMapChange();
	}

	void StandardMaterial::BuildUnformBuffer()
	{
		if(m_UniformBlock)
			return;

		m_UniformBlock = CreateRef<UniformBlock>();
		m_UniformBlock->PushBack("Albedo", UniformBlock::CreateData(UniformType::Vec3, "Albedo"));
		m_UniformBlock->PushBack("Emission", UniformBlock::CreateData(UniformType::Vec3, "Emission"));
		m_UniformBlock->PushBack("Metalness", UniformBlock::CreateData(UniformType::Float, "Metalness"));
		m_UniformBlock->PushBack("Roughness", UniformBlock::CreateData(UniformType::Float, "Roughness"));
		m_UniformBlock->PushBack("Occlusion", UniformBlock::CreateData(UniformType::Float, "Occlusion"));
		m_UniformBlock->PushBack("Alpha", UniformBlock::CreateData(UniformType::Float, "Alpha"));
		m_UniformBlock->PushBack("UseNormalMap", UniformBlock::CreateData(UniformType::Bool, "UseNormalMap"));
		m_UniformBlock->Calculate();

		m_UniformBuffer = UniformBuffer::Create(m_Context.lock(), m_UniformBlock->GetByteUsed(), 
												UBBinding_PBRMaterial, BufferUsage::Dynamic);
	}

	void StandardMaterial::OnAlbedoChange()
	{
		if(m_UniformBlock)
		{
			auto albedoUD = (*m_UniformBlock)["Albedo"];
			m_UniformBlock->WriteAt(glm::value_ptr(m_Albedo), sizeof(glm::vec3), albedoUD->GetAlignOffset());
		}		
	}

	void StandardMaterial::OnEmissionChange()
	{
		if(m_UniformBlock)
		{
			auto emissionUD = (*m_UniformBlock)["Emission"];
			m_UniformBlock->WriteAt(glm::value_ptr(m_Emission), sizeof(glm::vec3), emissionUD->GetAlignOffset());
		}
	}

	void StandardMaterial::OnMetallicChange()
	{
		if(m_UniformBlock)
		{
			auto metalnessUD = (*m_UniformBlock)["Metalness"];
			m_UniformBlock->WriteAt(&m_Metallic, sizeof(float), metalnessUD->GetAlignOffset());
		}
	}

	void StandardMaterial::OnRoughnessChange()
	{
		if(m_UniformBlock)
		{
			auto roughnessUD = (*m_UniformBlock)["Roughness"];
			m_UniformBlock->WriteAt(&m_Roughness, sizeof(float), roughnessUD->GetAlignOffset());
		}
	}

	void StandardMaterial::OnOcclusionChange()
	{
		if(m_UniformBlock)
		{			
			auto occlusionUD = (*m_UniformBlock)["Occlusion"];
			m_UniformBlock->WriteAt(&m_Occlusion, sizeof(float), occlusionUD->GetAlignOffset());
		}
	}

	void StandardMaterial::OnAlphaChange()
	{
		if(m_UniformBlock)
		{
			auto alphaUD = (*m_UniformBlock)["Alpha"];
			m_UniformBlock->WriteAt(&m_Alpha, sizeof(float), alphaUD->GetAlignOffset());
		}
	}

	void StandardMaterial::OnbUseNormalMapChange()
	{
		if(m_UniformBlock)
		{
			auto useNormalUD = (*m_UniformBlock)["UseNormalMap"];
			m_UniformBlock->WriteAt(&m_bUseNormalMap, sizeof(bool), useNormalUD->GetAlignOffset());
		}
	}
}