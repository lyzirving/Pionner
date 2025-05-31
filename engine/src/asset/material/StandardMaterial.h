#pragma once

#include "Material.h"

namespace pio
{
	class Image;

	class StandardMaterial : public Material
	{
		RTTR_ENABLE(Material)
	public:
		StandardMaterial(const ImportParams& params);
		virtual ~StandardMaterial() = default;		
		virtual bool BindAt(const Ref<Shader>& shader) override;
		virtual bool Bind() override;
		virtual bool UnBind() override;
		virtual Ref<Asset> SpawnChild() override;
		virtual bool IsTranslucency() const override { return !Math::Equal(m_Alpha, 1.f); }

		void SetAlbedoMap(const Ref<Image>& img) { m_AlbedoMap = img; }
		void SetMetallicRoughness(const Ref<Image>& img) { m_MetallicRoughness = img; }
		void SetNormalMap(const Ref<Image>& img) { m_NormalMap = img; SetbUseNormalMap(UseNormalMap()); }
		void SetOcclustionMap(const Ref<Image>& img) { m_OcclusionMap = img; }
		void SetEmissionMap(const Ref<Image>& img) { m_EmissionMap = img; }
		bool UseNormalMap() const { return m_NormalMap.use_count() != 0; }

		const Ref<Image>& GetAlbedoMap() const { return m_AlbedoMap; }
		const Ref<Image>& GetEmissionMap() const { return m_EmissionMap; }
		const Ref<Image>& GetOcclustionMap() const { return m_OcclusionMap; }
		const Ref<Image>& GetMetallicRoughnessMap() const { return m_MetallicRoughness; }
		const Ref<Image>& GetNormalMap() const { return m_NormalMap; }

	protected:
		virtual void OnInit() override;
		virtual void BuildUnformBuffer() override;

	private:
		Ref<Image> m_AlbedoMap;
		Ref<Image> m_MetallicRoughness;//R(ao)|G(roughness)|B(metallic)
		Ref<Image> m_NormalMap;
		Ref<Image> m_OcclusionMap;
		Ref<Image> m_EmissionMap;

		PIO_DEFINE_PROPERTY(glm::vec3, Albedo, 1.f)
		PIO_DEFINE_PROPERTY(glm::vec3, Emission, 1.f)
		PIO_DEFINE_PROPERTY(float, Metallic, 0.f)
		PIO_DEFINE_PROPERTY(float, Roughness, 0.5)
		PIO_DEFINE_PROPERTY(float, Occlusion, 1.f)
		PIO_DEFINE_PROPERTY(float, Alpha, 1.f)
		PIO_DEFINE_PROPERTY(bool, bUseNormalMap, false)
	};

}