#include "PbrMaterial.h"

#include "gfx/rhi/Shader.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PbrMaterial"

namespace pio
{
	PbrMaterial::PbrMaterial(const std::string& name, RenderingMode mode)
		: Material(name, mode, ShaderSpec_Standard)
	{
	}

	void PbrMaterial::update(Ref<RenderContext>& context)
	{
		updateUnimData(GpuAttr::AlbedoColor, m_albedo);
		updateUnimData(GpuAttr::Emission, m_emission);
		updateUnimData(GpuAttr::Metalness, m_metallic);
		updateUnimData(GpuAttr::Roughness, m_roughness);
		updateUnimData(GpuAttr::Alpha, m_alpha);
		updateUnimData(GpuAttr::UseNormalMap, m_normalMap.use_count() != 0);

		auto& textureMgr = context->textureMgr();
		updateTexture(GpuAttr::AlbedoTexture, m_albedoTexture.use_count() != 0 ? m_albedoTexture : textureMgr.get(pio::GpuAttr::WHITE_TEXTURE));
		updateTexture(GpuAttr::EmissionTexture, m_emissionTexture.use_count() != 0 ? m_emissionTexture : textureMgr.get(pio::GpuAttr::BLACK_TEXTURE));
		updateTexture(GpuAttr::MetallicRoughnssTexture, m_metallicRoughnessTexture.use_count() != 0 ? m_metallicRoughnessTexture : textureMgr.get(pio::GpuAttr::WHITE_TEXTURE));
		updateTexture(GpuAttr::NormalMap, m_normalMap.use_count() != 0 ? m_normalMap : textureMgr.get(pio::GpuAttr::BLACK_TEXTURE));
	}

	template<>
	bool Asset::is<PbrMaterial>() const
	{
		if (is<Material>())
		{
			return as<Material>()->spec() == ShaderSpec_Standard;
		}
		return false;
	}
}