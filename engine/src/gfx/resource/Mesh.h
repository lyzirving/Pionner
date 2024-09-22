#ifndef __PIONNER_GFX_RESOURCES_MESH_H__
#define __PIONNER_GFX_RESOURCES_MESH_H__

#include "asset/Asset.h"

#include "TriangleMesh.h"

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

		void setTriangleMesh(const TriangleMesh3d& mesh) { m_triangles = mesh; }
		void setTriangleMesh(TriangleMesh3d&& mesh) { m_triangles = std::forward<TriangleMesh3d>(mesh); }

		TriangleMesh3d& triangles() { return m_triangles; }
		const TriangleMesh3d& triangles() const { return m_triangles; }

	private:
		TriangleMesh3d m_triangles;

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