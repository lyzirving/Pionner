#ifndef __PIONNER_GFX_STRUCT_MATERIAL_ASSETS_H__
#define __PIONNER_GFX_STRUCT_MATERIAL_ASSETS_H__

#include "gfx/rhi/Material.h"
#include "asset/Asset.h"

namespace pio
{
	class MaterialAsset : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Material)
	public:
		MaterialAsset(const Ref<Material> &material);
		~MaterialAsset();

		const glm::vec3 &getAlbedoColor();
		float getMetalness();
		float getRoughness();
		float getEmission();

		Ref<Texture2D> getAlbedoMap();
		Ref<Texture2D> getNormalMap();
		Ref<Texture2D> getMetallicRoughnessMap();
		Ref<Texture2D> getEmissionMap();
		
		void setAlbedoColor(const glm::vec3 &color);
		void setMetalness(float value);
		void setRoughness(float value);
		void setEmission(float value);

		void setAlbedoMap(const Ref<Texture2D> &texture);
		void setMetallicRoughnessMap(const Ref<Texture2D> &texture);
		void setNormalMap(const Ref<Texture2D> &texture);
		void setEmissionMap(const Ref<Texture2D> &texture);

		const std::string &getName() const { return m_material->getName(); }
		Ref<Material> getMaterial() const { return m_material; }

		void setMaterial(const Ref<Material> &material) { m_material = material; }
		void setCastShadow(bool cast) { m_material->setFlag(MaterialFlag::ShadowCasting, cast); }
		bool isCastingShadow() { return m_material->getFlag(MaterialFlag::ShadowCasting); };

	private:
		Ref<Material> m_material;

	private:
		friend class MaterialLibrary;
	};

	class MaterialTable
	{
	public:
		MaterialTable(uint32_t materialCount = 1);
		MaterialTable(const Ref<MaterialTable> &other);
		~MaterialTable() = default;

		void set(uint32_t index, AssetHandle material);
		void clear(uint32_t index);
		void clearAll();

		bool exist(uint32_t index) const { return m_materials.find(index) != m_materials.end(); }
		AssetHandle operator[](uint32_t index) const { if (exist(index)) { return m_materials.at(index); } else { return AssetHandle(0); } }
		uint32_t getNum() const { return m_materialCount; }

		std::map<uint32_t, AssetHandle>::iterator begin() { return m_materials.begin(); }
		std::map<uint32_t, AssetHandle>::iterator end() { return m_materials.end(); }

	private:
		std::map<uint32_t, AssetHandle> m_materials;
		uint32_t m_materialCount;

	private:
		friend class MeshBase;
		friend class Mesh;
		friend class StaticMesh;
	};
}

#endif