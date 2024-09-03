#include "Material.h"

#include "gfx/rhi/UniformData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Material"

namespace pio
{
	Material::Material(const std::string &name) : Asset(), m_name(name)
	{
	}

	Material::Material(ShaderSpecifier spec, RenderingMode mode, const std::string &name)
		: Asset(), m_spec(spec), m_renderingMode(mode), m_name(name)
	{
	}

	template<>
	bool Asset::is<Material>() const { return assetType() == AssetType::Material; }
}