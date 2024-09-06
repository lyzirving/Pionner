#ifndef __PIONNER_SCENE_RESOURCES_MESH_H__
#define __PIONNER_SCENE_RESOURCES_MESH_H__

#include "TriangleMesh.h"
#include "asset/Asset.h"
#include "gfx/resource/MeshData.h"

namespace pio
{
	struct TransformComponent;
	class UniformData;

	class Mesh : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

		void setUp(Ref<RenderContext>& context);
		void update(const TransformComponent &comp);

		MeshData& data() { return m_data; }
		TriangleMesh& triMesh() { return m_triMesh; }
		std::map<std::string, Ref<UniformData>>& unims() { return m_uniforms; }

		const MeshData& data() const { return m_data; }	
		const TriangleMesh& triMesh() const { return m_triMesh; }
		const std::map<std::string, Ref<UniformData>>& unims() const { return m_uniforms; }

	protected:
		TriangleMesh m_triMesh;
		MeshData m_data;
		std::map<std::string, Ref<UniformData>> m_uniforms;

		Transform m_transform;		

	private:
		friend class Factory;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif