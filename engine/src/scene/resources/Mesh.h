#ifndef __PIONNER_SCENE_RESOURCES_MESH_H__
#define __PIONNER_SCENE_RESOURCES_MESH_H__

#include "TriangleMesh.h"
#include "asset/Asset.h"
#include "gfx/renderer/resource/MeshData.h"

namespace pio
{
	class Mesh : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

		void setUp(Ref<RenderContext>& context);

		MeshBuffer& buffer() { return m_buffer; }
		TriangleMesh& triMesh() { return m_triMesh; }

		const MeshBuffer& buffer() const { return m_buffer; }
		const TriangleMesh& triMesh() const { return m_triMesh; }

	protected:
		TriangleMesh m_triMesh;
		MeshBuffer m_buffer;

	private:
		friend class Factory;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif