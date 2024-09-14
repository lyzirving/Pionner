#include "Factory.h"
#include "Components.h"
#include "Scene.h"

#include "GlobalSettings.h"

#include "asset/AssetMgr.h"

#include "scene/3d/Camera.h"

#include "gfx/resource/Mesh.h"
#include "gfx/resource/Material.h"
#include "gfx/resource/MeshRenderBuffer.h"
#include "gfx/resource/RenderTarget.h"
#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/FrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Factory"

namespace pio
{
	Ref<Entity> Factory::MakeCamera(Ref<RenderContext>& context, Ref<Scene>& scene, const std::string& name, int32_t depth)
	{
		auto entity = scene->addEntity<CameraComponent, TransformComponent>(EntityType::Camera, name);
		auto* camComp = entity->getComponent<CameraComponent>();
		auto* transComp = entity->getComponent<TransformComponent>();

		auto camera = AssetMgr::MakeRuntimeAsset<Camera>();

		auto colorSize = GlobalSettings::ColorResolution();
		auto depthSize = GlobalSettings::ShadowResolution();
		FrameBufferSpecific fboSpec;
		fboSpec.Name = name + "-target";
		fboSpec.Width = colorSize.x;
		fboSpec.Height = colorSize.y;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Color);

		TextureSpecificBuilder texBuilder;
		texBuilder.name(fboSpec.Name + "-texture")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_HALF)
			.width(colorSize.x).height(colorSize.y)
			.texWrap(TextureWrap::ClampEdge, TextureWrap::ClampEdge)
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);

		TextureSpecificBuilder depthBuilder;
		depthBuilder.name(fboSpec.Name + "-depthStencil")
			.type(TextureType::RenderBuffer)
			.format(TextureFormat::DEPTH_24_STENCIL_8)
			.width(depthSize.x).height(depthSize.y);

		fboSpec.ColorSpec.push_back(texBuilder.build());
		fboSpec.DepthSpec = depthBuilder.build();

		camera->setRenderTarget(RenderTarget::Create(context, fboSpec));

		camComp->Depth = depth;
		camComp->Aspect = float(colorSize.x) / float(colorSize.y);
		camComp->Uid = camera->assetHnd();
		transComp->Position = glm::vec3(0.f, 3.f, 4.5f);
		transComp->Rotation = glm::vec3(-35.f, 0.f, 0.f);
		return entity;
	}

	Ref<Entity> Factory::MakePlane(Ref<RenderContext>& context, Ref<Scene>& scene, const std::string& name)
	{
		auto entity = scene->addEntity<MeshFilter, MeshRenderer, TransformComponent>(EntityType::Mesh, name);

		auto* meshFilter = entity->getComponent<MeshFilter>();
		auto* meshRender = entity->getComponent<MeshRenderer>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = GeometryFactory::MakePlane();

		meshFilter->Type = MeshType::Plane;
		meshFilter->Uid = mesh->assetHnd();

		meshRender->MatUid = context->getMaterial(GpuAttr::STANDARD_MATERIAL)->assetHnd();
		meshRender->BuffUid = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>()->assetHnd();

		return entity;
	}

	Ref<Entity> Factory::MakeCube(Ref<RenderContext>& context, Ref<Scene>& scene, const std::string& name)
	{
		auto entity = scene->addEntity<MeshFilter, MeshRenderer, TransformComponent>(EntityType::Mesh, name);

		auto* meshFilter = entity->getComponent<MeshFilter>();
		auto* meshRender = entity->getComponent<MeshRenderer>();
		auto* transform = entity->getComponent<TransformComponent>();

		auto mesh = AssetMgr::MakeRuntimeAsset<Mesh>();
		mesh->m_triangles = GeometryFactory::MakeCube();

		meshFilter->Type = MeshType::Cube;
		meshFilter->Uid = mesh->assetHnd();

		meshRender->MatUid = context->getMaterial(GpuAttr::STANDARD_MATERIAL)->assetHnd();
		meshRender->BuffUid = AssetMgr::MakeRuntimeAsset<MeshRenderBuffer>()->assetHnd();

		transform->Position = glm::vec3(0.f, 1.f, 0.f);

		return entity;
	}

	TriangleMesh3d GeometryFactory::MakePlane(float n)
	{
		TriangleMesh3d triangles;
		glm::vec3 origin(-n / 2.f, 0.f, -n / 2.f);
		uint32_t count = n;
		for(uint32_t row = 0; row < count; row++)
		{
			for(uint32_t col = 0; col < count; col++)
			{
				Vertex3d v0, v1, v2, v3;
				v0.Pos = origin + World::Right * float(col) + World::Forward * float(row);
				v1.Pos = v0.Pos + World::Forward;
				v2.Pos = v1.Pos + World::Right;
				v3.Pos = v2.Pos - World::Forward;

				v0.TexCoord = glm::vec2(float(col) / n, 1.f - float(row) / n);
				v1.TexCoord = glm::vec2(float(col) / n, 1.f - float(row + 1) / n);
				v2.TexCoord = glm::vec2(float(col + 1) / n, 1.f - float(row + 1) / n);
				v3.TexCoord = glm::vec2(float(col + 1) / n, 1.f - float(row) / n);

				v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;				

				MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
			}
		}
		return triangles;
	}

	TriangleMesh3d GeometryFactory::MakeCube(float n)
	{
		TriangleMesh3d triangles;
		float half = n * 0.5f;
		//front face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Right + World::Up) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}

		//right face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Right + World::Up + World::Forward) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Forward * n;
			v3.Pos = v0.Pos - World::Forward * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Right;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}

		//Left face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos + World::Forward * n;
			v3.Pos = v0.Pos + World::Forward * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Right;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}

		//Back face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward + World::Right) * half;
			v1.Pos = v0.Pos - World::Up * n;
			v2.Pos = v1.Pos - World::Right * n;
			v3.Pos = v0.Pos - World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Forward;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}

		//Top face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Up - World::Forward - World::Right) * half;
			v1.Pos = v0.Pos + World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Up;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}

		//Bottom face
		{
			Vertex3d v0, v1, v2, v3;
			v0.Pos = (World::Forward - World::Up - World::Right) * half;
			v1.Pos = v0.Pos - World::Forward * n;
			v2.Pos = v1.Pos + World::Right * n;
			v3.Pos = v0.Pos + World::Right * n;

			v0.TexCoord = glm::vec2(0.f, 1.f);
			v1.TexCoord = glm::vec2(0.f);
			v2.TexCoord = glm::vec2(1.f, 0.f);
			v3.TexCoord = glm::vec2(1.f, 1.f);

			v0.Normal = v1.Normal = v2.Normal = v3.Normal = -World::Up;
			MakeTriangleMesh3d(v0, v1, v2, v3, triangles);
		}
		return triangles;
	}

	TriangleMesh3d GeometryFactory::MakeScreenQuad()
	{
		TriangleMesh3d triangles;
		Vertex3d v0, v1, v2, v3;
		v0.Pos = glm::vec3(-1.f, 1.f, 0.f);
		v1.Pos = glm::vec3(-1.f, -1.f, 0.f);
		v2.Pos = glm::vec3(1.f, -1.f, 0.f);
		v3.Pos = glm::vec3(1.f, 1.f, 0.f);

		v0.TexCoord = glm::vec2(0.f, 1.f);
		v1.TexCoord = glm::vec2(0.f, 0.f);
		v2.TexCoord = glm::vec2(1.f, 0.f);
		v3.TexCoord = glm::vec2(1.f, 1.f);

		v0.Normal = v1.Normal = v2.Normal = v3.Normal = World::Forward;

		uint16_t idx = triangles.Vertices.size();
		triangles.Vertices.push_back(v0);
		triangles.Vertices.push_back(v1);
		triangles.Vertices.push_back(v2);
		triangles.Vertices.push_back(v3);

		triangles.Indices.push_back(idx);
		triangles.Indices.push_back(idx + 1);
		triangles.Indices.push_back(idx + 2);
		triangles.Indices.push_back(idx + 2);
		triangles.Indices.push_back(idx + 3);
		triangles.Indices.push_back(idx);

		triangles.Triangles.push_back(Triangle((v0.Normal + v1.Normal + v2.Normal) / 3.f,
									  { idx, (uint16_t)(idx + 1), (uint16_t)(idx + 2) }));
		triangles.Triangles.push_back(Triangle((v2.Normal + v3.Normal + v0.Normal) / 3.f,
									  { (uint16_t)(idx + 2), (uint16_t)(idx + 3), idx }));

		return triangles;
	}

	void GeometryFactory::MakeTriangleMesh3d(const Vertex3d& v0, const Vertex3d& v1, const Vertex3d& v2, const Vertex3d& v3, TriangleMesh3d& triangles)
	{
		uint16_t idx = triangles.Vertices.size();
		triangles.Vertices.push_back(v0);
		triangles.Vertices.push_back(v1);
		triangles.Vertices.push_back(v2);
		triangles.Vertices.push_back(v3);

		triangles.Indices.push_back(idx);
		triangles.Indices.push_back(idx + 1);
		triangles.Indices.push_back(idx + 2);
		triangles.Indices.push_back(idx + 2);
		triangles.Indices.push_back(idx + 3);
		triangles.Indices.push_back(idx);

		triangles.Triangles.push_back(Triangle((v0.Normal + v1.Normal + v2.Normal) / 3.f,
									  { idx, (uint16_t)(idx + 1), (uint16_t)(idx + 2) }));
		triangles.Triangles.push_back(Triangle((v2.Normal + v3.Normal + v0.Normal) / 3.f,
									  { (uint16_t)(idx + 2), (uint16_t)(idx + 3), idx }));
	}
}