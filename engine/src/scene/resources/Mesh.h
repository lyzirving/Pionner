#ifndef __PIONNER_SCENE_RESOURCES_MESH_H__
#define __PIONNER_SCENE_RESOURCES_MESH_H__

#include "TriangleMesh.h"

#include "asset/Asset.h"

namespace pio
{
	class RenderContext;
	struct TransformComponent;

	class Mesh : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Mesh)
	public:
		Mesh() : Asset() {}
		virtual ~Mesh() = default;

		void update(Ref<RenderContext>& context, const TransformComponent& comp);

		void setTriangleMesh(const TriangleMesh& mesh) { m_triangles = mesh; }
		void setTriangleMesh(TriangleMesh&& mesh) { m_triangles = std::forward<TriangleMesh>(mesh); }

		TriangleMesh& triangles() { return m_triangles; }
		Transform3D& transform() { return m_transform; }

		const TriangleMesh& triangles() const { return m_triangles; }
		const Transform3D& transform() const { return m_transform; }

	private:
		TriangleMesh m_triangles;
		Transform3D m_transform;

	private:
		friend class Factory;
	};

	template<>
	bool Asset::is<Mesh>() const;
}

#endif