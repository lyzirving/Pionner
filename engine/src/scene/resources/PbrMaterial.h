#ifndef __PIONNER_SCENE_RESOURCES_PBR_MATERIAL_H__
#define __PIONNER_SCENE_RESOURCES_PBR_MATERIAL_H__

#include "Material.h"

namespace pio
{
	class Texture;

	class PbrMaterial : public Material
	{
	public:
		PbrMaterial(const std::string& name, RenderingMode mode);
		~PbrMaterial() = default;
		virtual void update(Ref<RenderContext>& context) override;

		void setAlbedo(const glm::vec3& albedo) { m_albedo = albedo; }
		void setEmission(const glm::vec3& emission) { m_emission = emission; }
		void setAlpha(float alpha) { m_alpha = alpha; }
		void setMetallic(float metallic) { m_metallic = metallic; }
		void setRoughness(float roughness) { m_roughness = roughness; }
		void setSmoothness(float smoothness) { m_roughness = 1.f - smoothness; }

		void setAlbedoTexture(const Ref<Texture>& texture) { m_albedoTexture = texture; }
		void setEmissionTexture(const Ref<Texture>& texture) { m_emissionTexture = texture; }
		void setMetallicRoughnessTexture(const Ref<Texture>& texture) { m_metallicRoughnessTexture = texture; }
		void setNormalMap(const Ref<Texture>& texture) { m_normalMap = texture; }
		void setHeightMap(const Ref<Texture>& texture) { m_heightMap = texture; }
		void setOcclusion(const Ref<Texture>& texture) { m_occlusion = texture; }

	public:	
		struct GpuAttr
		{
			static constexpr char* AlbedoColor = "u_pbrMaterial.AlbedoColor";
			static constexpr char* Emission = "u_pbrMaterial.Emission";
			static constexpr char* Metalness = "u_pbrMaterial.Metalness";
			static constexpr char* Roughness = "u_pbrMaterial.Roughness";
			static constexpr char* Alpha = "u_pbrMaterial.Alpha";
			static constexpr char* UseNormalMap = "u_pbrMaterial.UseNormalMap";

			static constexpr char* AlbedoTexture = "u_pbrMaterial.AlbedoTexture";
			static constexpr char* EmissionTexture = "u_pbrMaterial.EmissionTexture";
			static constexpr char* MetallicRoughnssTexture = "u_pbrMaterial.MetallicRoughnessTexture";
			static constexpr char* NormalMap = "u_pbrMaterial.NormalMap";
		};

	private:		
		glm::vec3 m_albedo{ 1.f };
		glm::vec3 m_emission{ 0.f };
		float m_alpha{ 1.f };
		float m_metallic{ 0.f };
		float m_roughness{ 0.5f };// 1 - smoothness

		Ref<Texture> m_albedoTexture;
		Ref<Texture> m_emissionTexture;
		Ref<Texture> m_metallicRoughnessTexture;
		Ref<Texture> m_normalMap;
		//[TODO]
		Ref<Texture> m_heightMap;
		Ref<Texture> m_occlusion;
	};

	template<>
	bool Asset::is<PbrMaterial>() const;
}

#endif