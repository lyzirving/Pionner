#include "MeshFactory.h"

#include "MaterialLibrary.h"
#include "Geometry.h"
#include "Geometry2D.h"

#include "asset/AssetsManager.h"
#include "ui/UiDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MeshFactory"

namespace pio
{
	static uint32_t s_cubeNum{ 0 };
	static uint32_t s_planeNum{ 0 };
	static uint32_t s_cylinderNum{ 0 };
	static uint32_t s_coneNum{ 0 };
	static uint32_t s_arrowNum{ 0 };
	static uint32_t s_fullScreenQuadNum{ 0 };
	static uint32_t s_sphereNum{ 0 };

	Ref<MeshSource> MeshFactory::CreateCube(float len)
	{
		float radius = 0.5f * len;
		std::string name = std::string("Cube") + std::to_string(s_cubeNum++);
		Ref<Geometry> cube = RefCast<Asset, Geometry>(AssetsManager::CreateRuntimeAssets<Geometry>(name));

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Color);
		cube->m_materials.push_back(ma->getMaterial());

		cube->m_submeshes.emplace_back();
		Submesh &submesh = cube->m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;
		submesh.MeshName = name + "@mesh";
		submesh.BoundingBox = AABB(glm::vec3(-radius), glm::vec3(radius));
		cube->m_boundingBox = submesh.BoundingBox;

		std::vector<Vertex> vertex;
		std::vector<uint32_t> indices;
		uint32_t ind{ 0 };
		Vertex v{};
		Index face0{}, face1{};

		cube->m_vertices.reserve(6 * 6);
		cube->m_indices.reserve(6 * 2);

		// back face
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Normal = glm::vec3(0.f, 0.f, -1.f);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, -radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, -radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		ind++;

		// left face
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Normal = glm::vec3(-1.f, 0.f, 0.f);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, -radius);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		ind++;

		// right face
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Normal = glm::vec3(1.f, 0.f, 0.f);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, -radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		ind++;

		// front face
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Normal = glm::vec3(0.f, 0.f, 1.f);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		ind++;

		// top face
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Normal = glm::vec3(0.f, 1.f, 0.f);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, -radius);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, radius, -radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		ind++;

		// bottom face
		v.Position = glm::vec3(-radius, -radius, -radius);
		v.Normal = glm::vec3(0.f, -1.f, 0.f);
		v.Texcoord = glm::vec2(1.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, -radius);
		v.Texcoord = glm::vec2(0.f, 0.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(-radius, -radius, radius);
		v.Texcoord = glm::vec2(1.f, 1.f);
		cube->m_vertices.push_back(v);
		v.Position = glm::vec3(radius, -radius, radius);
		v.Texcoord = glm::vec2(0.f, 1.f);
		cube->m_vertices.push_back(v);

		face0 = Index(ind * 6 + 0, ind * 6 + 1, ind * 6 + 2);
		face1 = Index(ind * 6 + 3, ind * 6 + 4, ind * 6 + 5);
		cube->m_indices.push_back(face0);
		cube->m_indices.push_back(face1);

		submesh.VertexCount = cube->m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = cube->m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			cube->m_triangleCache[i].emplace_back(cube->m_vertices[cube->m_indices[i].V0 + submesh.VertexOffset],
												  cube->m_vertices[cube->m_indices[i].V1 + submesh.VertexOffset],
												  cube->m_vertices[cube->m_indices[i].V2 + submesh.VertexOffset]);
		}

		CreateBuffer<Vertex, Index>(cube);

		return cube;
	}

	Ref<MeshSource> MeshFactory::CreatePlane(float width, float height)
	{
		//TODO: dynamically add material
		std::string name = std::string("Plane") + std::to_string(s_planeNum++);
		auto meshSource = RefCast<Asset, MeshSource>(AssetsManager::CreateRuntimeAssets<MeshSource>(name));

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Packed_SandyRock);
		meshSource->m_materials.push_back(ma->getMaterial());

		meshSource->m_submeshes.emplace_back();
		Submesh &submesh = meshSource->m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.VertexCount = 4;
		submesh.IndexCount = 2;
		submesh.MeshName = name + "@mesh";

		auto &aabb = submesh.BoundingBox;
		aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
		aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		Vertex lt;
		lt.Position = glm::vec3(-width * 0.5f, 0.f, -height * 0.5f);
		lt.Normal = glm::vec3(0.f, 1.f, 0.f);
		lt.Texcoord = glm::vec2(0.f, 1.f);

		aabb.Min = glm::min(aabb.Min, lt.Position);
		aabb.Max = glm::max(aabb.Max, lt.Position);

		Vertex lb;
		lb.Position = glm::vec3(-width * 0.5f, 0.f, height * 0.5f);
		lb.Normal = glm::vec3(0.f, 1.f, 0.f);
		lb.Texcoord = glm::vec2(0.f, 0.f);

		aabb.Min = glm::min(aabb.Min, lb.Position);
		aabb.Max = glm::max(aabb.Max, lb.Position);

		Vertex rt;
		rt.Position = glm::vec3(width * 0.5f, 0.f, -height * 0.5f);
		rt.Normal = glm::vec3(0.f, 1.f, 0.f);
		rt.Texcoord = glm::vec2(1.f, 1.f);

		aabb.Min = glm::min(aabb.Min, rt.Position);
		aabb.Max = glm::max(aabb.Max, rt.Position);

		Vertex rb;
		rb.Position = glm::vec3(width * 0.5f, 0.f, height * 0.5f);
		rb.Normal = glm::vec3(0.f, 1.f, 0.f);
		rb.Texcoord = glm::vec2(1.f, 0.f);

		aabb.Min = glm::min(aabb.Min, rb.Position);
		aabb.Max = glm::max(aabb.Max, rb.Position);

		meshSource->m_vertices.reserve(4);
		meshSource->m_vertices.emplace_back(std::move(lt));
		meshSource->m_vertices.emplace_back(std::move(lb));
		meshSource->m_vertices.emplace_back(std::move(rt));
		meshSource->m_vertices.emplace_back(std::move(rb));

		meshSource->m_indices.reserve(2);
		meshSource->m_indices.emplace_back(0, 1, 2);
		meshSource->m_indices.emplace_back(2, 1, 3);

		meshSource->m_boundingBox = aabb;

		meshSource->m_triangleCache[0].emplace_back(meshSource->m_vertices[meshSource->m_indices[0].V0 + submesh.VertexOffset],
													meshSource->m_vertices[meshSource->m_indices[0].V1 + submesh.VertexOffset],
													meshSource->m_vertices[meshSource->m_indices[0].V2 + submesh.VertexOffset]);

		meshSource->m_triangleCache[1].emplace_back(meshSource->m_vertices[meshSource->m_indices[1].V0 + submesh.VertexOffset],
													meshSource->m_vertices[meshSource->m_indices[1].V1 + submesh.VertexOffset],
													meshSource->m_vertices[meshSource->m_indices[1].V2 + submesh.VertexOffset]);

		CreateBuffer<Vertex, Index>(meshSource);

		return meshSource;
	}

	Ref<MeshSource> MeshFactory::CreateCylinder(float radius, float height, uint32_t itrCnt)
	{
		std::string name = std::string("Cylinder") + std::to_string(s_cylinderNum++);
		Ref<Cylinder> cylinder = RefCast<Asset, Cylinder>(AssetsManager::CreateRuntimeAssets<Cylinder>(name));
		// Bottom center is (0.f, 0.f, 0.f)
		cylinder->m_height = height;
		cylinder->m_radius = radius;

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Color);
		cylinder->m_materials.push_back(ma->getMaterial());

		cylinder->m_submeshes.emplace_back();
		Submesh &submesh = cylinder->m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.MeshName = name + "@mesh";
		submesh.BoundingBox = AABB(glm::vec3(-radius, 0.f, -radius), glm::vec3(radius, height, radius));
		cylinder->m_boundingBox = submesh.BoundingBox;

		std::vector<Vertex> unitCircleVertex;
		{
			float step = 2.f * PIO_PI / float(itrCnt);
			float angle;
			Vertex v;
			unitCircleVertex.reserve(itrCnt + 1);
			for (uint32_t i = 0; i <= itrCnt; i++)
			{
				angle = i * step;
				v.Position = glm::vec3(radius * std::sin(angle), 0.f, radius * std::cos(angle));
				v.Normal = glm::normalize(glm::vec3(std::sin(angle), 0.f, std::cos(angle)));
				unitCircleVertex.push_back(v);
			}
		}

		// Top and bottom face vertex
		std::vector<Vertex> topVertex, bottomVertex;
		topVertex.resize(unitCircleVertex.size());
		bottomVertex.resize(unitCircleVertex.size());
		for (uint32_t i = 0; i < unitCircleVertex.size(); i++)
		{
			bottomVertex[i] = topVertex[i] = unitCircleVertex[i];
			topVertex[i].Position.y = height;
		}

		int32_t idx{ -1 };
		// Side face
		for (uint32_t i = 0; i < topVertex.size() - 1; ++i)
		{
			// Left triangle
			cylinder->m_vertices.push_back(topVertex[i]);
			cylinder->m_vertices.push_back(bottomVertex[i]);
			cylinder->m_vertices.push_back(bottomVertex[i + 1]);
			Index leftIdx(idx + 1, idx + 2, idx + 3);
			cylinder->m_indices.push_back(leftIdx);
			idx += 3;

			// Right triangle
			cylinder->m_vertices.push_back(topVertex[i]);
			cylinder->m_vertices.push_back(bottomVertex[i + 1]);
			cylinder->m_vertices.push_back(topVertex[i + 1]);
			Index rightIdx(idx + 1, idx + 2, idx + 3);
			cylinder->m_indices.push_back(rightIdx);
			idx += 3;
		}

		// Top face triangle
		for (int i = 0; i < topVertex.size() - 1; ++i)
		{
			Vertex center;
			center.Position = glm::vec3(0.f, height, 0.f);
			center.Normal = glm::vec3(0.f, 1.f, 0.f);
			cylinder->m_vertices.push_back(center);
			cylinder->m_vertices.push_back(topVertex[i]);
			cylinder->m_vertices.push_back(topVertex[i + 1]);
			Index topIdx(idx + 1, idx + 2, idx + 3);
			cylinder->m_indices.push_back(topIdx);
			idx += 3;
		}

		// Bottom face triangle
		for (int i = 0; i < bottomVertex.size() - 1; ++i)
		{
			Vertex center;
			center.Position = glm::vec3(0.f, 0.f, 0.f);
			center.Normal = glm::vec3(0.f, -1.f, 0.f);
			cylinder->m_vertices.push_back(center);
			cylinder->m_vertices.push_back(bottomVertex[i + 1]);
			cylinder->m_vertices.push_back(bottomVertex[i]);
			Index topIdx(idx + 1, idx + 2, idx + 3);
			cylinder->m_indices.push_back(topIdx);
			idx += 3;
		}

		submesh.VertexCount = cylinder->m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = cylinder->m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			cylinder->m_triangleCache[i].emplace_back(cylinder->m_vertices[cylinder->m_indices[i].V0 + submesh.VertexOffset],
													  cylinder->m_vertices[cylinder->m_indices[i].V1 + submesh.VertexOffset],
													  cylinder->m_vertices[cylinder->m_indices[i].V2 + submesh.VertexOffset]);
		}

		CreateBuffer<Vertex, Index>(cylinder);

		return cylinder;
	}

	Ref<MeshSource> MeshFactory::CreateCone(float radius, float height, uint32_t itrCnt)
	{
		std::string name = std::string("Cone") + std::to_string(s_coneNum++);
		Ref<Cone> cone = RefCast<Asset, Cone>(AssetsManager::CreateRuntimeAssets<Cone>(name));
		// Bottom center is (0.f, 0.f, 0.f)
		cone->m_height = height;
		cone->m_radius = radius;

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Color);
		cone->m_materials.push_back(ma->getMaterial());

		cone->m_submeshes.emplace_back();
		Submesh &submesh = cone->m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.MeshName = name + "@mesh";
		submesh.BoundingBox = AABB(glm::vec3(-radius, 0.f, -radius), glm::vec3(radius, height, radius));
		cone->m_boundingBox = submesh.BoundingBox;

		// Bottom face vertex
		std::vector<Vertex> bottomVertex;
		{
			float step = 2.f * PIO_PI / float(itrCnt);
			float angle;
			Vertex v;
			bottomVertex.reserve(itrCnt + 1);
			for (uint32_t i = 0; i <= itrCnt; i++)
			{
				angle = i * step;
				v.Position = glm::vec3(radius * std::sin(angle), 0.f, radius * std::cos(angle));
				v.Normal = glm::normalize(glm::vec3(std::sin(angle), 0.f, std::cos(angle)));
				bottomVertex.push_back(v);
			}
		}

		int32_t idx{ -1 };
		{
			// Side face triangle
			Vertex topVertex;
			topVertex.Position = glm::vec3(0.f, height, 0.f);
			topVertex.Normal = glm::vec3(0.f, 1.f, 0.f);			
			for (uint32_t i = 0; i < bottomVertex.size() - 1; ++i)
			{
				cone->m_vertices.push_back(topVertex);
				cone->m_vertices.push_back(bottomVertex[i]);
				cone->m_vertices.push_back(bottomVertex[i + 1]);
				Index faceIdx(idx + 1, idx + 2, idx + 3);
				cone->m_indices.push_back(faceIdx);
				idx += 3;
			}
		}

		{
			// Bottom face triangle
			Vertex center;
			center.Position = glm::vec3(0.f, 0.f, 0.f);
			center.Normal = glm::vec3(0.f, -1.f, 0.f);
			for (int i = 0; i < bottomVertex.size() - 1; ++i)
			{				
				cone->m_vertices.push_back(center);
				cone->m_vertices.push_back(bottomVertex[i + 1]);
				cone->m_vertices.push_back(bottomVertex[i]);
				Index faceIdx(idx + 1, idx + 2, idx + 3);
				cone->m_indices.push_back(faceIdx);
				idx += 3;
			}
		}

		submesh.VertexCount = cone->m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = cone->m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			cone->m_triangleCache[i].emplace_back(cone->m_vertices[cone->m_indices[i].V0 + submesh.VertexOffset],
												  cone->m_vertices[cone->m_indices[i].V1 + submesh.VertexOffset],
												  cone->m_vertices[cone->m_indices[i].V2 + submesh.VertexOffset]);
		}

		CreateBuffer<Vertex, Index>(cone);

		return cone;
	}

	Ref<MeshSource> MeshFactory::CreateArrow3D(float cylinderRadius, float cylinderHeight, float coneRadius, float coneHeight, uint32_t itrCnt)
	{
		std::string name = std::string("Arrow3D") + std::to_string(s_arrowNum++);
		Ref<Arrow3D> arrow = RefCast<Asset, Arrow3D>(AssetsManager::CreateRuntimeAssets<Arrow3D>(name));
		// Bottom center is (0.f, 0.f, 0.f)
		arrow->m_cylinRadius = cylinderRadius;
		arrow->m_cylinHeight = cylinderHeight;
		arrow->m_coneRadius = coneRadius;
		arrow->m_coneHeight = coneHeight;

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Color);
		arrow->m_materials.push_back(ma->getMaterial());

		arrow->m_submeshes.emplace_back();
		Submesh &submesh = arrow->m_submeshes.back();
		submesh.VertexOffset = 0;
		submesh.IndexOffset = 0;
		submesh.MaterialIndex = 0;// one mesh has one material
		submesh.MeshName = name + "@mesh";
		float radiusMax = std::max(coneRadius, cylinderRadius);
		submesh.BoundingBox = AABB(glm::vec3(-radiusMax, 0.f, -radiusMax), glm::vec3(radiusMax, coneHeight + cylinderHeight, radiusMax));
		arrow->m_boundingBox = submesh.BoundingBox;

		int32_t idx{ -1 };
		// Cylinder
		{
			std::vector<Vertex> unitCircleVertex;
			{
				float step = 2.f * PIO_PI / float(itrCnt);
				float angle;
				Vertex v;
				unitCircleVertex.reserve(itrCnt + 1);
				for (uint32_t i = 0; i <= itrCnt; i++)
				{
					angle = i * step;
					v.Position = glm::vec3(cylinderRadius * std::sin(angle), 0.f, cylinderRadius * std::cos(angle));
					v.Normal = glm::normalize(glm::vec3(std::sin(angle), 0.f, std::cos(angle)));
					unitCircleVertex.push_back(v);
				}
			}

			// Top and bottom face vertex
			std::vector<Vertex> topVertex, bottomVertex;
			topVertex.resize(unitCircleVertex.size());
			bottomVertex.resize(unitCircleVertex.size());
			for (uint32_t i = 0; i < unitCircleVertex.size(); i++)
			{
				bottomVertex[i] = topVertex[i] = unitCircleVertex[i];
				topVertex[i].Position.y = cylinderHeight;
			}

			// Side face
			for (uint32_t i = 0; i < topVertex.size() - 1; ++i)
			{
				// Left triangle
				arrow->m_vertices.push_back(topVertex[i]);
				arrow->m_vertices.push_back(bottomVertex[i]);
				arrow->m_vertices.push_back(bottomVertex[i + 1]);
				Index leftIdx(idx + 1, idx + 2, idx + 3);
				arrow->m_indices.push_back(leftIdx);
				idx += 3;

				// Right triangle
				arrow->m_vertices.push_back(topVertex[i]);
				arrow->m_vertices.push_back(bottomVertex[i + 1]);
				arrow->m_vertices.push_back(topVertex[i + 1]);
				Index rightIdx(idx + 1, idx + 2, idx + 3);
				arrow->m_indices.push_back(rightIdx);
				idx += 3;
			}

			// Top face triangle
			for (int i = 0; i < topVertex.size() - 1; ++i)
			{
				Vertex center;
				center.Position = glm::vec3(0.f, cylinderHeight, 0.f);
				center.Normal = glm::vec3(0.f, 1.f, 0.f);
				arrow->m_vertices.push_back(center);
				arrow->m_vertices.push_back(topVertex[i]);
				arrow->m_vertices.push_back(topVertex[i + 1]);
				Index topIdx(idx + 1, idx + 2, idx + 3);
				arrow->m_indices.push_back(topIdx);
				idx += 3;
			}

			// Bottom face triangle
			for (int i = 0; i < bottomVertex.size() - 1; ++i)
			{
				Vertex center;
				center.Position = glm::vec3(0.f, 0.f, 0.f);
				center.Normal = glm::vec3(0.f, -1.f, 0.f);
				arrow->m_vertices.push_back(center);
				arrow->m_vertices.push_back(bottomVertex[i + 1]);
				arrow->m_vertices.push_back(bottomVertex[i]);
				Index topIdx(idx + 1, idx + 2, idx + 3);
				arrow->m_indices.push_back(topIdx);
				idx += 3;
			}
		}

		// Cone
		{
			// Bottom face vertex
			std::vector<Vertex> bottomVertex;
			{
				float step = 2.f * PIO_PI / float(itrCnt);
				float angle;
				Vertex v;
				bottomVertex.reserve(itrCnt + 1);
				for (uint32_t i = 0; i <= itrCnt; i++)
				{
					angle = i * step;
					v.Position = glm::vec3(coneRadius * std::sin(angle), cylinderHeight, coneRadius * std::cos(angle));
					v.Normal = glm::normalize(glm::vec3(std::sin(angle), 0.f, std::cos(angle)));
					bottomVertex.push_back(v);
				}
			}

			{
				// Side face triangle
				Vertex topVertex;
				topVertex.Position = glm::vec3(0.f, cylinderHeight + coneHeight, 0.f);
				topVertex.Normal = glm::vec3(0.f, 1.f, 0.f);
				for (uint32_t i = 0; i < bottomVertex.size() - 1; ++i)
				{
					arrow->m_vertices.push_back(topVertex);
					arrow->m_vertices.push_back(bottomVertex[i]);
					arrow->m_vertices.push_back(bottomVertex[i + 1]);
					Index faceIdx(idx + 1, idx + 2, idx + 3);
					arrow->m_indices.push_back(faceIdx);
					idx += 3;
				}
			}

			{
				// Bottom face triangle
				Vertex center;
				center.Position = glm::vec3(0.f, cylinderHeight, 0.f);
				center.Normal = glm::vec3(0.f, -1.f, 0.f);
				for (int i = 0; i < bottomVertex.size() - 1; ++i)
				{
					arrow->m_vertices.push_back(center);
					arrow->m_vertices.push_back(bottomVertex[i + 1]);
					arrow->m_vertices.push_back(bottomVertex[i]);
					Index faceIdx(idx + 1, idx + 2, idx + 3);
					arrow->m_indices.push_back(faceIdx);
					idx += 3;
				}
			}
		}

		submesh.VertexCount = arrow->m_vertices.size();
		uint32_t faceNum = submesh.IndexCount = arrow->m_indices.size();

		for (uint32_t i = 0; i < faceNum; i++)
		{
			arrow->m_triangleCache[i].emplace_back(arrow->m_vertices[arrow->m_indices[i].V0 + submesh.VertexOffset],
												   arrow->m_vertices[arrow->m_indices[i].V1 + submesh.VertexOffset],
												   arrow->m_vertices[arrow->m_indices[i].V2 + submesh.VertexOffset]);
		}

		CreateBuffer<Vertex, Index>(arrow);

		return arrow;
	}

	Ref<MeshSource> MeshFactory::CreateSphere(float radius, const glm::vec3 &color, uint32_t horItrCnt, uint32_t verItrCnt)
	{
		std::string name = std::string("Sphere") + std::to_string(s_sphereNum++);
		Ref<Sphere> sphere = RefCast<Asset, Sphere>(AssetsManager::CreateRuntimeAssets<Sphere>(name));
		sphere->m_radius = radius;
		sphere->m_horItrCnt = horItrCnt;
		sphere->m_verItrCnt = verItrCnt;

		Ref<MaterialAsset> ma = MaterialLibrary::Get()->getMaterial(MaterialType_Color);
		sphere->m_materials.push_back(ma->getMaterial());
		sphere->setColor(color);

		sphere->build();

		CreateBuffer<Vertex, Index>(sphere);

		sphere->invalidate(false);

		return sphere;
	}

	Ref<Geometry2D> MeshFactory::CreateFullScreenQuad()
	{
		std::string name = std::string("FullScreenQuad") + std::to_string(s_fullScreenQuadNum++);
		auto quad = RefCast<Asset, QuadMesh>(AssetsManager::CreateRuntimeAssets<QuadMesh>(name));

		QuadVertex lt;
		lt.Position = glm::vec3(-1.f, 1.f, 0.f);
		lt.Texcoord = glm::vec2(0.f, 1.f);

		QuadVertex lb;
		lb.Position = glm::vec3(-1.f, -1.f, 0.f);
		lb.Texcoord = glm::vec2(0.f, 0.f);

		QuadVertex rt;
		rt.Position = glm::vec3(1.f, 1.f, 0.f);
		rt.Texcoord = glm::vec2(1.f, 1.f);

		QuadVertex rb;
		rb.Position = glm::vec3(1.f, -1.f, 0.f);
		rb.Texcoord = glm::vec2(1.f, 0.f);

		quad->Vertex.reserve(4);
		quad->Vertex.emplace_back(std::move(lt));
		quad->Vertex.emplace_back(std::move(lb));
		quad->Vertex.emplace_back(std::move(rt));
		quad->Vertex.emplace_back(std::move(rb));

		quad->Indices.reserve(6);
		quad->Indices.push_back(0);
		quad->Indices.push_back(1);
		quad->Indices.push_back(2);
		quad->Indices.push_back(2);
		quad->Indices.push_back(1);
		quad->Indices.push_back(3);

		quad->VertexBuffer = VertexBuffer::Create(quad->Vertex.data(), quad->Vertex.size() * sizeof(QuadVertex));
		quad->VertexBuffer->setLayout(VertexBuffer::To<QuadVertex>());

		quad->IndexBuffer = IndexBuffer::Create(quad->Indices.data(),
												quad->Indices.size() * sizeof(uint32_t),
												quad->Indices.size());

		quad->VertexArray = VertexArray::Create();
		quad->VertexArray->addVertexBuffer(quad->VertexBuffer);

		return quad;
	}

	Ref<Geometry2D> MeshFactory::CreateScreenQuad(uint32_t l, uint32_t t, uint32_t r, uint32_t b, uint32_t screenWidth, uint32_t screenHeight)
	{
		std::string name = std::string("ScreenQuad") + std::to_string(s_fullScreenQuadNum++);
		auto quad = RefCast<Asset, QuadMesh>(AssetsManager::CreateRuntimeAssets<QuadMesh>(name));

		glm::vec2 p = UiDef::ScreenToVertex(l, t, screenWidth, screenHeight);
		QuadVertex lt;
		lt.Position = glm::vec3(p.x, p.y, 0.f);
		lt.Texcoord = glm::vec2(0.f, 1.f);

		p = UiDef::ScreenToVertex(l, b, screenWidth, screenHeight);
		QuadVertex lb;
		lb.Position = glm::vec3(p.x, p.y, 0.f);
		lb.Texcoord = glm::vec2(0.f, 0.f);

		p = UiDef::ScreenToVertex(r, t, screenWidth, screenHeight);
		QuadVertex rt;
		rt.Position = glm::vec3(p.x, p.y, 0.f);
		rt.Texcoord = glm::vec2(1.f, 1.f);

		p = UiDef::ScreenToVertex(r, b, screenWidth, screenHeight);
		QuadVertex rb;
		rb.Position = glm::vec3(p.x, p.y, 0.f);
		rb.Texcoord = glm::vec2(1.f, 0.f);

		quad->Vertex.reserve(4);
		quad->Vertex.emplace_back(std::move(lt));
		quad->Vertex.emplace_back(std::move(lb));
		quad->Vertex.emplace_back(std::move(rt));
		quad->Vertex.emplace_back(std::move(rb));

		quad->Indices.reserve(6);
		quad->Indices.push_back(0);
		quad->Indices.push_back(1);
		quad->Indices.push_back(2);
		quad->Indices.push_back(2);
		quad->Indices.push_back(1);
		quad->Indices.push_back(3);

		quad->VertexBuffer = VertexBuffer::Create(quad->Vertex.data(), quad->Vertex.size() * sizeof(QuadVertex));
		quad->VertexBuffer->setLayout(VertexBuffer::To<QuadVertex>());

		quad->IndexBuffer = IndexBuffer::Create(quad->Indices.data(),
												quad->Indices.size() * sizeof(uint32_t),
												quad->Indices.size());

		quad->VertexArray = VertexArray::Create();
		quad->VertexArray->addVertexBuffer(quad->VertexBuffer);

		return quad;
	}
}