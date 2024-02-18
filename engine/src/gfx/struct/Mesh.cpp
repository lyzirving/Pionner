#include "Mesh.h"
#include "MaterialAsset.h"

#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Material.h"

#include "asset/AssetsManager.h"

#include "animation/Animation.h"
#include "animation/Skeleton.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Mesh"

namespace pio
{
	void BoneTransformUD::serialize()
	{
		if (!Block.m_buffer || Block.m_buffer->getCapacity() == 0)
		{
			LOGE("Bone Transform UD buffer is invalid");
			return;
		}
		auto transformUD = Block.m_blockItems.get("Transform");
		if (transformUD)
		{
			Block.m_buffer->writeAt(glm::value_ptr(Transform[0]), 
									sizeof(glm::mat4) * transformUD->getArrayNum(),
									transformUD->getAlignOffset());
		}
	}

	UniformBlock BoneTransformUD::CreateBlock()
	{
		UniformBlock block;
		block.pushBack("Transform", UniformBlock::CreateData(UniformDataType::Mat4Array, "Transform", PIO_MAX_BONE_MATRIX));
		block.calculate();
		return block;
	}

	MeshSource::MeshSource(const std::string &name) : Asset(name)
	{
	}

	MeshSource::~MeshSource()
	{
		m_skeleton.reset();
		m_boneTransformUB.reset();

		auto it = m_animations.begin();
		while (it != m_animations.end())
		{
			(*it).reset();
			it = m_animations.erase(it);
		}
	}

	void BoneInfluence::normalizeWeights()
	{
		float sumWeights = 0.0f;
		for (size_t i = 0; i < PIO_MAX_BONE_INFLUENCE; i++)
		{
			if (BoneInfoIndices[i] == NullIndex)
				continue;
			sumWeights += Weights[i];
		}
		if (sumWeights > 0.f)
		{
			for (size_t i = 0; i < PIO_MAX_BONE_INFLUENCE; i++)
			{
				if (BoneInfoIndices[i] == NullIndex)
					continue;
				Weights[i] /= sumWeights;
			}
		}
	}

	StaticMesh::StaticMesh(const Ref<MeshSource> &meshSource) : MeshBase(meshSource)
	{
	}

	StaticMesh::StaticMesh(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex)
		: MeshBase(meshSource, submeshIndex)
	{
	}

	Mesh::Mesh(const Ref<MeshSource> &meshSource) : MeshBase(meshSource)
	{
	}

	Mesh::Mesh(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex)
		: MeshBase(meshSource, submeshIndex)
	{
	}

	MeshBase::MeshBase() : Asset()
	{
	}

	MeshBase::MeshBase(const Ref<MeshSource> &meshSource) : Asset(), m_meshSource(meshSource)
	{
		setSubmeshIndex({});
		createMaterialTable(meshSource->m_materials);
	}

	MeshBase::MeshBase(const Ref<MeshSource> &meshSource, const std::vector<uint32_t> &submeshIndex)
		: Asset(), m_meshSource(meshSource)
	{
		setSubmeshIndex(submeshIndex);
		createMaterialTable(meshSource->m_materials);
	}

	MeshBase::~MeshBase()
	{
		m_meshSource.reset();
		m_materialTable.reset();
	}

	void MeshBase::createMaterialTable(const std::vector<Ref<Material>> &materials)
	{
		m_materialTable = CreateRef<MaterialTable>(materials.size());
		for (uint32_t i = 0; i < materials.size(); i++)
		{
			m_materialTable->set(i, AssetsManager::CreateRuntimeAssets<MaterialAsset>(materials[i])->getHandle());
		}
	}

	void MeshBase::setSubmeshIndex(const std::vector<uint32_t> &submeshIndex)
	{
		if (submeshIndex.empty())
		{
			auto &sub = m_meshSource->getSubmeshes();
			m_submeshIndex.resize(sub.size());
			for (size_t i = 0; i < sub.size(); i++)
				m_submeshIndex[i] = i;
		}
		else
		{
			m_submeshIndex.assign(submeshIndex.begin(), submeshIndex.end());
		}
	}

	void MeshBase::setMaterial(const std::vector<Ref<Material>> &materials)
	{
		if (!materials.empty())
		{
			createMaterialTable(materials);
		}
	}

	template<>
	bool Asset::is<MeshSource>() const { return this->getAssetType() == AssetType::MeshSource; }

	template<>
	bool Asset::is<Mesh>() const { return this->getAssetType() == AssetType::Mesh; }

	template<>
	bool Asset::is<StaticMesh>() const { return this->getAssetType() == AssetType::StaticMesh; }
}