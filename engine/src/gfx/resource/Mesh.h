#ifndef __PIONNER_GFX_RESOURCES_MESH_H__
#define __PIONNER_GFX_RESOURCES_MESH_H__

#include "asset/Asset.h"

#include "MeshData.h"

namespace pio
{
	class RenderContext;
	struct TransformComponent;

	class Mesh : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

		void setTriangleMesh(const TriangleMesh& mesh) { m_triangles = mesh; }
		void setTriangleMesh(TriangleMesh&& mesh) { m_triangles = std::forward<TriangleMesh>(mesh); }

		TriangleMesh& triangles() { return m_triangles; }
		const TriangleMesh& triangles() const { return m_triangles; }

	private:
		TriangleMesh m_triangles;

	private:
		friend class Factory;
		friend class PlaneNode;
		friend class CubeNode;
		friend class DirectionalLightNode;
		friend class SpriteNode;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif