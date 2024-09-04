#ifndef __PIONNER_SCENE_RESOURCES_PBR_MATERIAL_H__
#define __PIONNER_SCENE_RESOURCES_PBR_MATERIAL_H__

#include "Material.h"

namespace pio
{
	class Texture2D;

	class PbrMaterial : public Material
	{
	public:
		PbrMaterial(const std::string& name, RenderingMode mode);
		~PbrMaterial() = default;

		virtual void bind(Ref<Shader>& shader) override;

		void setAlbedo(const glm::vec3& albedo) { m_albedo = albedo; }
		void setAlpha(float alpha) { m_alpha = alpha; }
		void setMetallic(float metallic) { m_metallic = metallic; }
		void setRoughness(float roughness) { m_roughness = roughness; }
		void setSmoothness(float smoothness) { m_roughness = 1 - smoothness; }

		void setAlbedoTexture(const Ref<Texture2D>& texture) { m_albedoTexture = texture; }
		void setMetallicRoughnessTexture(const Ref<Texture2D>& texture) { m_metallicRoughnessTexture = texture; }
		void setNormalMap(const Ref<Texture2D>& texture) { m_normalMap = texture; }
		void setHeightMap(const Ref<Texture2D>& texture) { m_heightMap = texture; }
		void setOcclusion(const Ref<Texture2D>& texture) { m_occlusion = texture; }

	private:		
		glm::vec3 m_albedo{ 1.f };
		float m_alpha{ 1.f };
		float m_metallic{ 0.f };
		float m_roughness{ 0.5f };// 1 - smoothness

		Ref<Texture2D> m_albedoTexture;
		Ref<Texture2D> m_metallicRoughnessTexture;
		Ref<Texture2D> m_normalMap;
		//[TODO]
		Ref<Texture2D> m_heightMap;
		Ref<Texture2D> m_occlusion;
	};

	template<>
	bool Asset::is<PbrMaterial>() const;
}

#endif