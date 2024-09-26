#ifndef __PIONNER_GFX_RESOURCES_MESH_H__
#define __PIONNER_GFX_RESOURCES_MESH_H__

#include "asset/Asset.h"

#include "MeshData.h"

namespace pio
{
	class Mesh : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

		void setData(const Ref<MeshDataBase>& data) { m_data = data; }

		Ref<MeshDataBase>& data() { return m_data; }
		const Ref<MeshDataBase>& data() const { return m_data; }

	private:
		Ref<MeshDataBase> m_data;

	private:
		friend class Factory;
		friend class PlaneNode;
		friend class CubeNode;		
		friend class SpriteNode;
		friend class DirectionalLightNode;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif