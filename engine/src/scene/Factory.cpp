#include "Factory.h"

#include "Components.h"
#include "Scene.h"

#include "asset/AssetMgr.h"

#include "scene/3d/Camera.h"
#include "scene/3d/Mesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	Ref<Entity> Factory::MakeCamera(Ref<Scene>& scene, const std::string& name, int32_t depth)
	{
		auto entity	= scene->addEntity<CameraComponent>(name);
		auto *pComp = entity->getComponent<CameraComponent>();
		pComp->Depth = depth;
		pComp->Handle = AssetMgr::MakeRuntimeAsset<Camera>()->id();
		return entity;
	}

	Ref<Entity> Factory::MakePlane(Ref<Scene>& scene, const std::string& name)
	{
		auto entity = scene->addEntity<MeshFilter, MeshRenderer>(name);
		auto* meshFilter = entity->getComponent<MeshFilter>();
		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triMesh = Geometry3dFactory::MakePlane();

		meshFilter->Type = MeshType::Plane;
		meshFilter->Handle = mesh->id();
		return entity;
	}

	TriangleMesh Geometry3dFactory::MakePlane(uint32_t n)
	{
		TriangleMesh triMesh;
		glm::vec3 origin(-float(n) / 2.f, 0.f, -float(n) / 2.f);
		for (uint32_t row = 0; row < n; row++)
		{
			for (uint32_t col = 0; col < n; col++)
			{
				Vertex3d v0, v1, v2, v3;
				v0.Pos = origin + k_AxisX * float(col) + k_AxisZ * float(row);
				v1.Pos = v0.Pos + k_AxisZ;
				v2.Pos = v1.Pos + k_AxisX;
				v3.Pos = v2.Pos - k_AxisZ;

				uint32_t idx = triMesh.Vertices.size();
				triMesh.Vertices.push_back(v0);
				triMesh.Vertices.push_back(v1);
				triMesh.Vertices.push_back(v2);
				triMesh.Vertices.push_back(v3);

				triMesh.Indices.push_back(idx);
				triMesh.Indices.push_back(idx + 1);
				triMesh.Indices.push_back(idx + 2);
				triMesh.Indices.push_back(idx + 2);
				triMesh.Indices.push_back(idx + 3);
				triMesh.Indices.push_back(idx);
			}
		}
		return triMesh;
	}
}