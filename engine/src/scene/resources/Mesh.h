#ifndef __PIONNER_SCENE_RESOURCES_MESH_H__
#define __PIONNER_SCENE_RESOURCES_MESH_H__

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

		void update(Ref<RenderContext>& context, const TransformComponent& comp);

		void setTriangleMesh(const TriangleMesh3d& mesh) { m_triangles = mesh; }
		void setTriangleMesh(TriangleMesh3d&& mesh) { m_triangles = std::forward<TriangleMesh3d>(mesh); }

		TriangleMesh3d& triangles() { return m_triangles; }
		Transform3D& transform() { return m_transform; }

		const TriangleMesh3d& triangles() const { return m_triangles; }
		const Transform3D& transform() const { return m_transform; }

	private:
		TriangleMesh3d m_triangles;
		Transform3D m_transform;

	private:
		friend class Factory;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif