#include "Material.h"

#include "PbrMaterial.h"
#include "SpriteMaterial.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"

#include "asset/AssetMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Material"

namespace pio
{
	Material::Material(const std::string& name, RenderingMode mode, ShaderSpecifier spec)
		: Asset(), m_name(name), m_renderingMode(mode), m_spec(spec)
	{
	}

	Material::Material(const Material& rhs) : Asset(rhs)
	{
		m_name = rhs.m_name;
		m_renderingMode = rhs.m_renderingMode;
		m_spec = rhs.m_spec;

		m_uniforms = rhs.m_uniforms;
		m_textures = rhs.m_textures;
	}

	Material& Material::operator=(const Material& rhs)
	{
		if (this != &rhs)
		{
			this->Material::Material(rhs);
		}
		return *this;
	}

	void Material::bind(Ref<Shader>& shader)
	{
		for (auto& it : m_uniforms)
		{
			shader->setUniformData(it.second);
		}

		TextureSampler slot;
		for (auto& it : m_textures)
		{
			if (it.second->is<Texture2D>())
			{
				auto *p = it.second->as<Texture2D>();
				if (shader->getSampler(slot))
				{
					p->active(slot);
					p->bind();
					shader->setTextureSampler(it.first, slot);
				}
				else
				{
					LOGE("err! fail to get available sampler for material[%s]", m_name.c_str());
					break;
				}
			}			
		}		
	}

	Ref<Material> Material::Create(const std::string& name, ShaderSpecifier spec, RenderingMode mode)
	{
		switch(spec)
		{
			case ShaderSpec_Standard:
				return AssetMgr::MakeRuntimeAsset<PbrMaterial>(name, mode);
			case ShaderSpec_Sprite:
				return AssetMgr::MakeRuntimeAsset<SpriteMaterial>(name, mode);
			default:
				LOGE("err! shader spec[%u] has not been implemented", spec);
				return Ref<Material>();
		}
	}	

	template<>
	bool Asset::is<Material>() const { return assetType() == AssetType::Material; }
}