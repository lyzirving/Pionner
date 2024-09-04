#include "Material.h"

#include "PbrMaterial.h"

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

	Ref<Material> Material::MakeStandardMaterial(const std::string& name, RenderingMode mode)
	{
		return Material::Create(name, mode, ShaderSpec_Standard);
	}

	Ref<Material> Material::Create(const std::string& name, RenderingMode mode, ShaderSpecifier spec)
	{
		switch (spec)
		{
			case ShaderSpec_Standard:
				return AssetMgr::MakeRuntimeAsset<PbrMaterial>(name, mode);				
			default:
				LOGE("err! shader spec[%u] has not been implemented", spec);
				return Ref<Material>();
		}		
	}	

	template<>
	bool Asset::is<Material>() const { return assetType() == AssetType::Material; }
}