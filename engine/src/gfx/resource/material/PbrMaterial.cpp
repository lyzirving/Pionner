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

	PbrMaterial::PbrMaterial(const PbrMaterial& rhs) : Material(rhs)
	{
		m_albedo = rhs.m_albedo;
		m_emission = rhs.m_emission;		
		m_metallic = rhs.m_metallic;		
		m_smoothness = rhs.m_smoothness;
		m_occlusion = rhs.m_occlusion;
		m_alpha = rhs.m_alpha;

		m_albedoTexture = rhs.m_albedoTexture;
		m_emissionTexture = rhs.m_emissionTexture;
		m_metallicRoughness = rhs.m_metallicRoughness;
		m_normalMap = rhs.m_normalMap;
		m_heightMap = rhs.m_heightMap;
	}

	PbrMaterial& PbrMaterial::operator=(const PbrMaterial& rhs)
	{
		if (this != &rhs)
		{
			this->PbrMaterial::PbrMaterial(rhs);
		}
		return *this;
	}

	void PbrMaterial::onUpdate(Ref<RenderContext>& context)
	{			
		updateTexture(GpuAttr::AlbedoTexture, m_albedoTexture.use_count() != 0 ? m_albedoTexture : context->getTexture(pio::GpuAttr::Tex::WHITE));
		updateTexture(GpuAttr::EmissionTexture, m_emissionTexture.use_count() != 0 ? m_emissionTexture : context->getTexture(pio::GpuAttr::Tex::BLACK));
		updateTexture(GpuAttr::MetallicRoughnss, m_metallicRoughness.use_count() != 0 ? m_metallicRoughness : context->getTexture(pio::GpuAttr::Tex::DEFAULT_METALLIC_ROUGHNESS));
		updateTexture(GpuAttr::NormalMap, m_normalMap.use_count() != 0 ? m_normalMap : context->getTexture(pio::GpuAttr::Tex::BLACK));
		updateTexture(GpuAttr::HeightMap, m_heightMap.use_count() != 0 ? m_heightMap : context->getTexture(pio::GpuAttr::Tex::BLACK));	

		updateUnimData(GpuAttr::AlbedoColor, m_albedo);
		updateUnimData(GpuAttr::Emission, m_emission);
		updateUnimData(GpuAttr::Metalness, m_metallic);
		updateUnimData(GpuAttr::Roughness, Smoothness2Roughness(m_smoothness));
		updateUnimData(GpuAttr::Occlusion, m_occlusion);
		updateUnimData(GpuAttr::Alpha, m_alpha);
		updateUnimData(GpuAttr::UseNormalMap, m_normalMap.use_count() != 0);
	}

	Ref<Asset> PbrMaterial::clone() const
	{
		auto material = CreateRef<PbrMaterial>(m_name, m_renderingMode);
		*(material.get()) = *this;		
		material->m_hnd = UUID32();
		return material;
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