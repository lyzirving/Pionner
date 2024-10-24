#ifndef __PIONNER_GFX_RESOURCES_MATERIAL_PBR_MATERIAL_H__
#define __PIONNER_GFX_RESOURCES_MATERIAL_PBR_MATERIAL_H__

#include "Material.h"

namespace pio
{
	class Texture;

	class PbrMaterial : public Material
	{
	public:
		PbrMaterial(const std::string& name, RenderingMode mode);
		PbrMaterial(const PbrMaterial& rhs);
		PbrMaterial& operator=(const PbrMaterial& rhs);
		~PbrMaterial() = default;
		virtual void onUpdate(Ref<RenderContext>& context) override;

		void setAlbedo(const glm::vec3& albedo) { m_albedo = albedo; }
		void setEmission(const glm::vec3& emission) { m_emission = emission; }
		void setAlpha(float alpha) { m_alpha = alpha; }
		void setMetallic(float metallic) { m_metallic = metallic; }
		void setSmoothness(float smoothness) { m_smoothness = smoothness; }
		void setOcclustion(float occlusion) { m_occlusion = occlusion; }

		void setAlbedoTexture(const Ref<Texture>& texture) { m_albedoTexture = texture; }
		void setEmissionTexture(const Ref<Texture>& texture) { m_emissionTexture = texture; }
		void setMetallicRoughness(const Ref<Texture>& texture) { m_metallicRoughness = texture; }
		void setNormalMap(const Ref<Texture>& texture) { m_normalMap = texture; }
		void setHeightMap(const Ref<Texture>& texture) { m_heightMap = texture; }		

		const glm::vec3& getAlbedo() const { return m_albedo; }
		float getMetallic() const { return m_metallic; }
		float getSmoothness() const { return m_smoothness; }

		const Ref<Texture>& getAlbedoTexture() const { return m_albedoTexture; }
		const Ref<Texture>& getMetallicRoughness() const { return m_metallicRoughness; }		

		virtual Ref<Asset> clone() const override;

	public:	
		struct GpuAttr
		{
			static constexpr char* AlbedoColor = "u_pbrMaterial.AlbedoColor";
			static constexpr char* Emission = "u_pbrMaterial.Emission";
			static constexpr char* Metalness = "u_pbrMaterial.Metalness";
			static constexpr char* Roughness = "u_pbrMaterial.Roughness";
			static constexpr char* Occlusion = "u_pbrMaterial.Occlusion";
			static constexpr char* Alpha = "u_pbrMaterial.Alpha";
			static constexpr char* UseNormalMap = "u_pbrMaterial.UseNormalMap";

			static constexpr char* AlbedoTexture = "u_pbrMaterial.AlbedoTexture";
			static constexpr char* EmissionTexture = "u_pbrMaterial.EmissionTexture";
			static constexpr char* MetallicRoughnss = "u_pbrMaterial.MetallicRoughness";			
			static constexpr char* NormalMap = "u_pbrMaterial.NormalMap";
			static constexpr char* HeightMap = "u_pbrMaterial.HeightMap";			
		};
	private:
		static float Smoothness2Roughness(float val) { return (1.f - val); }

	private:		
		glm::vec3 m_albedo{ 1.f };
		glm::vec3 m_emission{ 0.f };		
		float m_metallic{ 0.f };		
		float m_smoothness{ 0.5f };
		float m_occlusion{ 1.f };
		float m_alpha{ 1.f };

		Ref<Texture> m_albedoTexture;
		Ref<Texture> m_emissionTexture;
		Ref<Texture> m_metallicRoughness;//r(ao)|g(roughness)|b(metallic)
		Ref<Texture> m_normalMap;		
		Ref<Texture> m_heightMap;//@todo

	private:
		friend class MaterialImporter;
	};

	template<>
	bool Asset::is<PbrMaterial>() const;
}

#endif