#include "Material.h"

namespace pio
{
	Material::Material() : Asset()
	{
	}

	Material::Material(ShaderSpecifier spec, RenderingMode mode)
		: Asset(), m_shaderSpec(spec), m_renderingMode(mode)
	{
	}

	template<>
	bool Asset::is<Material>() const { return assetType() == AssetType::Material; }
}