#include "MaterialAsset.h"

namespace pio
{
	MaterialAsset::MaterialAsset(const Ref<Material> &material) : Asset(), m_material(material)
	{
	}

	MaterialAsset::~MaterialAsset()
	{
		m_material.reset();
	}

	const glm::vec3 &MaterialAsset::getAlbedoColor()
	{
		return m_material->getVector3(MaterialAttrs::MU_AlbedoColor);
	}

	float MaterialAsset::getMetalness()
	{
		return m_material->getFloat(MaterialAttrs::MU_Metalness);
	}

	float MaterialAsset::getRoughness()
	{
		return m_material->getFloat(MaterialAttrs::MU_Roughness);
	}

	float MaterialAsset::getEmission()
	{
		return m_material->getFloat(MaterialAttrs::MU_Emission);
	}

	Ref<Texture2D> MaterialAsset::getAlbedoMap()
	{
		return m_material->getTexture2D(MaterialAttrs::MU_AlbedoTexture);
	}

	Ref<Texture2D> MaterialAsset::getNormalMap()
	{
		return m_material->getTexture2D(MaterialAttrs::MU_NormalTexture);
	}

	Ref<Texture2D> MaterialAsset::getMetallicRoughnessMap()
	{
		return m_material->getTexture2D(MaterialAttrs::MU_MetallicRoughnessTexture);
	}

	void MaterialAsset::setAlbedoColor(const glm::vec3 &color)
	{
		m_material->set(MaterialAttrs::MU_AlbedoColor, color);
	}

	void MaterialAsset::setMetalness(float value)
	{
		m_material->set(MaterialAttrs::MU_Metalness, value);
	}

	void MaterialAsset::setRoughness(float value)
	{
		m_material->set(MaterialAttrs::MU_Roughness, value);
	}

	void MaterialAsset::setEmission(float value)
	{
		m_material->set(MaterialAttrs::MU_Emission, value);
	}

	void MaterialAsset::setAlbedoMap(const Ref<Texture2D> &texture)
	{
		m_material->set(MaterialAttrs::MU_AlbedoTexture, texture);
	}

	void MaterialAsset::setMetallicRoughnessMap(const Ref<Texture2D> &texture)
	{
		m_material->set(MaterialAttrs::MU_MetallicRoughnessTexture, texture);
	}

	void MaterialAsset::setNormalMap(const Ref<Texture2D> &texture)
	{
		m_material->set(MaterialAttrs::MU_NormalTexture, texture);
	}

	MaterialTable::MaterialTable(uint32_t materialCount) : m_materialCount(materialCount)
	{
	}

	MaterialTable::MaterialTable(const Ref<MaterialTable> &other) : m_materialCount(other->m_materialCount)
	{
		const auto &meshMaterials = other->m_materials;
		auto it = meshMaterials.begin();
		while (it != meshMaterials.end())
		{
			set(it->first, it->second);
			it++;
		}
	}

	void MaterialTable::set(uint32_t index, AssetHandle material)
	{
		m_materials[index] = material;
		if (index >= m_materialCount)
			m_materialCount = index + 1;
	}

	void MaterialTable::clear(uint32_t index)
	{
		if (exist(index))
		{
			m_materials.erase(index);
			if (index >= m_materialCount)
				m_materialCount = index + 1;
		}
	}

	void MaterialTable::clearAll()
	{
		m_materials.clear();
	}
}