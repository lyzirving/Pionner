#include "Factory.h"

#include "Components.h"
#include "Scene.h"

#include "asset/AssetMgr.h"

#include "scene/3d/Camera.h"
#include "scene/resources/Mesh.h"
#include "scene/resources/Material.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	Ref<Entity> Factory::MakeCamera(Ref<Scene>& scene, const std::string& name, int32_t depth)
	{
		auto entity	= scene->addEntity<CameraComponent, TransformComponent>(name);
		auto *pComp = entity->getComponent<CameraComponent>();
		pComp->Depth = depth;
		pComp->Uid = AssetMgr::MakeRuntimeAsset<Camera>()->assetHnd();
		return entity;
	}

	Ref<Entity> Factory::MakePlane(Ref<Scene>& scene, const std::string& name)
	{
		auto entity = scene->addEntity<MeshFilter, MeshRenderer, TransformComponent>(name);

		auto* meshFilter = entity->getComponent<MeshFilter>();
		auto* meshRender = entity->getComponent<MeshRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triMesh = Geometry3dFactory::MakePlane();
		
		auto material = AssetMgr::MakeRuntimeAsset<Material>("My Material");

		meshFilter->Type = MeshType::Plane;
		meshFilter->Uid = mesh->assetHnd();

		meshRender->MatUid = material->assetHnd();

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
				v0.Pos = origin + World::Right * float(col) + World::Forward * float(row);				
				v1.Pos = v0.Pos + World::Forward;			
				v2.Pos = v1.Pos + World::Right;
				v3.Pos = v2.Pos - World::Forward;

				v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;

				v0.TexCoord = glm::vec2(float(col) / float(n), 1.f - float(row) / float(n));
				v1.TexCoord = glm::vec2(float(col) / float(n), 1.f - float(row + 1) / float(n));
				v2.TexCoord = glm::vec2(float(col + 1) / float(n), 1.f - float(row + 1) / float(n));
				v3.TexCoord = glm::vec2(float(col + 1) / float(n), 1.f - float(row) / float(n));

				uint16_t idx = triMesh.Vertices.size();
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
				
				triMesh.Triangles.push_back(Triangle((v0.Normal + v1.Normal + v2.Normal) / 3.f,
											{ idx, (uint16_t)(idx + 1), (uint16_t)(idx + 2) }));
				triMesh.Triangles.push_back(Triangle((v2.Normal + v3.Normal + v0.Normal) / 3.f,
											{ (uint16_t)(idx + 2), (uint16_t)(idx + 3), idx }));
			}
		}
		return triMesh;
	}
}