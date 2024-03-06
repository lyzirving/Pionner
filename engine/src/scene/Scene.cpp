#include "Scene.h"
#include "Skybox.h"
#include "Application.h"

#include "gfx/renderer/SceneRenderer.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshUtil.h" 
#include "gfx/struct/MaterialLibrary.h"
#include "gfx/struct/Geometry2D.h"
#include "gfx/rhi/Texture.h"

#include "core/EventBus.h"
#include "core/math/Ray.h"

#include "ui/UiDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace pio
{
	Registry *Scene::s_registry = Registry::Get();

	static void LightCompToSceneData(DirectionalLightComponent &lightComp, TransformComponent &transComp, DirectionalLight &sceneData)
	{		
		sceneData.Position = transComp.Transform.Position;
		if (transComp.Transform.Euler.bDirty())
		{
			sceneData.Direction = glm::normalize(transComp.Transform.Euler.mat() * glm::vec4(lightComp.Direction, 0.f));
		}		
		sceneData.Radiance = lightComp.Radiance;
		sceneData.Intensity = lightComp.Intensity;
		sceneData.Bias = lightComp.Bias;
		sceneData.SdMode = lightComp.SdMode;
		sceneData.CastShadow = lightComp.CastShadow;
	}

	static void LightCompToSceneData(PointLightComponent &comp, PointLight &sceneData)
	{
		sceneData.Position = SphereCoord::ToCCS(comp.Position);
		sceneData.Radiance = comp.Radiance;
		sceneData.Intensity = comp.Intensity;
		sceneData.MinRadius = comp.MinRadius;
		sceneData.Radius = comp.Radius;
		sceneData.Falloff = comp.Falloff;
		sceneData.SourceSize = comp.SourceSize;

		Ref<StaticMesh> mesh = AssetsManager::GetRuntimeAsset<StaticMesh>(sceneData.Volume);
		if (mesh) { mesh->getMeshSource()->as<Sphere>()->setRadius(sceneData.Radius); }
	}

	static void CreatePointLightComponent(uint32_t index, PointLight &light, Ref<PhysicsScene> physicsScene)
	{
		Ref<Entity> ent = Registry::Get()->create<PointLightComponent>();
		auto &ptComp = ent->getComponent<PointLightComponent>();
		ptComp.Index = index;
		ptComp.Position = SphereCoord::ToSCS(light.Position);
		ptComp.Radiance = light.Radiance;
		ptComp.Intensity = light.Intensity;
		ptComp.MinRadius = light.MinRadius;
		ptComp.Radius = light.Radius;
		ptComp.Falloff = light.Falloff;
		ptComp.SourceSize = light.SourceSize;

		//TODO: use cache for model
		AssimpMeshImporter importer("bulb", AssetFmt::Obj);
		Ref<MeshSource> meshSrc = RefCast<Asset, MeshSource>(importer.importToMeshSource());
		meshSrc->GlobalPose.Position = ptComp.Position.toCartesian();
		MeshUtilParam param;
		param.State.Blend = Blend::Disable();
		param.State.Mode = RenderMode::MaterialPreview;
		param.Delay = true;
		param.Proxy = ent;
		Ref<MeshBase> mesh = RefCast<Asset, MeshBase>(CreateMesh<BoxColliderComponent>(meshSrc, physicsScene, RigidBodyComponent::Type::Dynamic, param));
		Ref<MaterialTable> mt = mesh->getMaterialTable();
		auto itr = mt->begin();
		while (itr != mt->end())
		{
			Ref<MaterialAsset> ma =	AssetsManager::GetRuntimeAsset<MaterialAsset>(itr->second);
			ma->setCastShadow(false);
			itr++;
		}

		ptComp.MeshHandle = mesh->getHandle();
	}

	Scene::Scene() : EventBusObject()
	{
		EventBus::Get()->addRegister(PioEvent::UnzipAsset, EventBusCb(this, (EventBusCbFunc)&Scene::onAssetUnzip));
	}

	Scene::~Scene()
	{
		EventBus::Get()->removeRegister(PioEvent::UnzipAsset, EventBusCb(this, (EventBusCbFunc)&Scene::onAssetUnzip));
	}

	void Scene::onAttach(Ref<SceneRenderer> &renderer)
	{
		createData();

		m_lightEnv.DirectionalLight.obtainBlock();
		m_lightEnv.DirectionalLightShadowData.obtainBlock();
		m_lightEnv.PointLightData.obtainBlock();
		m_lightEnv.PtLightShadowData.obtainBlock();

		uint32_t w = m_layoutParam.Viewport.Width;
		uint32_t h = m_layoutParam.Viewport.Height;
		m_screenQuad = MeshFactory::CreateScreenQuad(0, 0, w, h, w, h)->getHandle();

		renderer->onAttach(*this);
	}

	void Scene::onDetach(Ref<SceneRenderer> &renderer)
	{
		renderer->onDetach(*this);
	}

	void Scene::onUpdate(const Timestep &ts)
	{
		Camera &sceneCam = m_mainCameraEnt->getComponent<CameraComponent>().Camera;
		sceneCam.flush();

		simulate(ts);		

		// Process Animation
		{
			auto view = s_registry->view<AnimationComponent>();
			for (auto &v : view)
			{
				auto &entity = v.second;
				auto &animComp = entity->getComponent<AnimationComponent>();
				auto animAsset = AssetsManager::GetRuntimeAsset<AnimationAsset>(animComp.Handle);
				if (animAsset)
				{
					Ref<Animator> animator = animAsset->getAnimator();
					animator->setRunMode(animComp.GPUSkinning ? AnimRunMode::GPU_Run : AnimRunMode::CPU_Run);
					animator->play(animComp.Selection);
					animator->update(float(ts));
				}
			}
		}

		// Process Point Light
		{
			auto view = s_registry->view<PointLightComponent>();
			auto it = view.begin();
			while (it != view.end())
			{
				PointLightComponent &comp = it->second->getComponent<PointLightComponent>();
				LightCompToSceneData(comp, m_lightEnv.PointLightData.Lights[comp.Index]);
				Ref<MeshBase> mesh = AssetsManager::GetRuntimeAsset<MeshBase>(comp.MeshHandle);
				Ref<MeshSource> meshSrc = mesh->getMeshSource();
				meshSrc->GlobalPose.Position = comp.Position.toCartesian();
				it++;
			}
		}

		// Only one directional light in scene
		{
			auto view = s_registry->view<DirectionalLightComponent>();
			if (view.size() == 1)
			{
				Ref<Entity> ent = view.begin()->second;
				auto &lightComp = ent->getComponent<DirectionalLightComponent>();
				auto &transComp = ent->getComponent<TransformComponent>();
				LightCompToSceneData(lightComp, transComp, m_lightEnv.DirectionalLight);
			}
			else
			{
				LOGE("Err! Mutiple distant light");
			}			
		}
	}

	void Scene::onRender(Ref<SceneRenderer> &renderer, const Timestep &ts)
	{
		// -------------------- Render 3D scene ----------------------------
		// -----------------------------------------------------------------
		Camera &sceneCam = m_mainCameraEnt->getComponent<CameraComponent>().Camera;
		renderer->beginScene(*this, sceneCam);

		// Process dynamic mesh
		{
			auto view = s_registry->view<MeshComponent, TransformComponent>();
			for (auto &v : view)
			{
				auto &entity = v.second;
				auto &meshComp = entity->getComponent<MeshComponent>();
				if (meshComp.SubmeshIndex == NullIndex || !meshComp.Visible)
					continue;

				Ref<Mesh> mesh = AssetsManager::GetRuntimeAsset<Mesh>(meshComp.Handle);
				Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);

				auto &transComp = entity->getComponent<TransformComponent>();
				Transform transform = meshSrc->GlobalPose * transComp.Transform;
				entity->setActorGlobalPose(transform.Position, transform.Euler.quat());
				if (meshComp.Delay)
					renderer->submitDelayedMesh(Ref<MeshBase>(mesh), meshComp.SubmeshIndex, transform, meshComp.State);
				else
					renderer->submitMesh(Ref<MeshBase>(mesh), meshComp.SubmeshIndex, transform, meshComp.State);
			}
		}

		// Process static mesh
		{
			auto view = s_registry->view<StaticMeshComponent, TransformComponent>();
			for (auto &v : view)
			{
				auto &entity = v.second;
				auto &staticMeshComp = entity->getComponent<StaticMeshComponent>();
				if (staticMeshComp.SubmeshIndex == NullIndex || !staticMeshComp.Visible)
					continue;

				Ref<StaticMesh> staticMesh = AssetsManager::GetRuntimeAsset<StaticMesh>(staticMeshComp.Handle);
				Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(staticMeshComp.SourceHandle);

				auto &transComp = entity->getComponent<TransformComponent>();
				Transform transform = meshSrc->GlobalPose * transComp.Transform;
				entity->setActorGlobalPose(transform.Position, transform.Euler.quat());
				if (staticMeshComp.Delay)
					renderer->submitDelayedMesh(Ref<MeshBase>(staticMesh), staticMeshComp.SubmeshIndex, transform, staticMeshComp.State);
				else
					renderer->submitMesh(Ref<MeshBase>(staticMesh), staticMeshComp.SubmeshIndex, transform, staticMeshComp.State);
			}
		}
		
		// Sprite
		{
			auto view = s_registry->view<SpriteComponent>();
			for (auto &v : view)
			{
				auto &entity = v.second;
				auto &comp = entity->getComponent<SpriteComponent>();
				if (comp.QuadMesh == NullIndex || !comp.Visible)
					continue;

				renderer->submitSprite(comp.QuadMesh, comp.Texture, comp.State);
			}
		}

		renderer->endScene(*this);
	}

	void Scene::moveCamera(float viewPosDiffX, float viewPosDiffY)
	{
		Camera &camera = m_mainCameraEnt->getComponent<CameraComponent>().Camera;
		camera.addPosDiff(viewPosDiffX, viewPosDiffY);
	}

	void Scene::setCameraViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		Camera &camera = m_mainCameraEnt->getComponent<CameraComponent>().Camera;
		camera.setViewport(x, y, width, height);
		camera.setAspect(float(width) / float(height));
	}

	void Scene::createData()
	{
		// Scene and Physics
		m_sceneRoot = s_registry->create<SceneComponent, RelationshipComponent>(NodeType::Scene);
		PIO_RELATION_SET_TAG(m_sceneRoot, "MyScene");
		PIO_RELATION_SET_SELF_INDEX(m_sceneRoot, m_sceneRoot->getCacheIndex());
		auto &sceneComp = m_sceneRoot->getComponent<SceneComponent>();
		sceneComp.Primary = true;
		sceneComp.Simulate = false;
		// NOTE: create PhysicsScene should be called at last
		auto physicsScene = CreateRef<PhysicsScene>("Main");
		AssetsManager::Get()->addRuntimeAsset(physicsScene);
		sceneComp.PhycisScene = physicsScene->getHandle();

		// Main Camera
		m_mainCameraEnt = s_registry->create<CameraComponent>();
		auto &cameraComp = m_mainCameraEnt->getComponent<CameraComponent>();
		cameraComp.Camera.setPosition(72.f, 341.f, 10.f);
		cameraComp.Camera.setLookAt(glm::vec3(0.f));
		cameraComp.Primary = true;

		// Distant Light
		{
			m_lightEnv.DirectionalLight = DirectionalLight(glm::vec3(-4.5f, 3.8f, -1.f), glm::vec3(0.f), glm::vec3(3.f), 0.12f);

			Ref<Entity> ent = Registry::Get()->create<DirectionalLightComponent, RelationshipComponent, SpriteComponent, TransformComponent>(NodeType::DistantLight);
			auto &transComp = ent->getComponent<TransformComponent>();
			auto &lightComp = ent->getComponent<DirectionalLightComponent>();
			transComp.Transform.Position = m_lightEnv.DirectionalLight.Position;
			lightComp.Direction = m_lightEnv.DirectionalLight.Direction;
			lightComp.Radiance = m_lightEnv.DirectionalLight.Radiance;
			lightComp.Intensity = m_lightEnv.DirectionalLight.Intensity;

			auto &rlComp = ent->getComponent<RelationshipComponent>();
			PIO_RELATION_SET_TAG(ent, "MainLight");
			PIO_RELATION_SET_SELF_INDEX(ent, ent->getCacheIndex());
			PIO_RELATION_SET_PARENT_INDEX(ent, m_sceneRoot->getCacheIndex());
			PIO_RELATION_SET_CHILD_INDEX(m_sceneRoot, ent->getCacheIndex());

			auto &spriteComp = ent->getComponent<SpriteComponent>();
			spriteComp.Visible = true;
			spriteComp.Name = rlComp.Tag;	
			spriteComp.QuadMesh = MeshFactory::CreateScreenQuad(0, 0, 1, 1, 1, 1)->getHandle();
			TextureSpecification spec; spec.SRGB = true; 
			Ref<Texture2D> icon = Texture2D::Create(AssetsManager::SpriteAbsPath("distant_light", AssetFmt::PNG), spec);
			spriteComp.Texture = icon->getHandle();
			AssetsManager::Get()->addRuntimeAsset(icon);
			spriteComp.State.DepthTest = DepthTest::Disable();
			spriteComp.State.Blend = Blend::Common();
			spriteComp.State.Cull = CullFace::Common();
			spriteComp.State.Stencil.Enable = false;

			Renderer::SubmitTask([icon]() mutable { icon->init(); });
		}

		//Point Light
		{						
			m_lightEnv.PointLightData.LightCount = 1;
			m_lightEnv.PtLightShadowData.LightCount = m_lightEnv.PointLightData.LightCount;

			m_lightEnv.PointLightData.Lights[0] = PointLight(SphereCoord::ToCCS(43.f, 63.f, 3.7f),
															 glm::vec3(3.f), 1.f, 0.001f, 5.f, 0.5f, 0.1f);
			m_lightEnv.PointLightData.Lights[0].Volume = AssetsManager::CreateRuntimeAssets<StaticMesh>(MeshFactory::CreateSphere(m_lightEnv.PointLightData.Lights[0].Radius))->getHandle();
			m_lightEnv.PointLightData.Lights[1] = PointLight(SphereCoord::ToCCS(40.f, 290.f, 3.2f),
															 glm::vec3(3.f), 1.f, 0.001f, 5.f, 0.5f, 0.1f);			
			m_lightEnv.PointLightData.Lights[1].Volume = AssetsManager::CreateRuntimeAssets<StaticMesh>(MeshFactory::CreateSphere(m_lightEnv.PointLightData.Lights[1].Radius))->getHandle();
			for (uint32_t i = 0; i < m_lightEnv.PointLightData.LightCount; i++)
				CreatePointLightComponent(i, m_lightEnv.PointLightData.Lights[i], physicsScene);
		}

		// Plane
		{
			Ref<MeshSource> planeMs = MeshFactory::CreatePlane();
			Ref<Asset> planeMsAst = AssetsManager::CreateRuntimeAssets<StaticMesh>(planeMs);
			const std::vector<Submesh> &submeshes = planeMs->getSubmeshes();
			for (uint32_t i = 0; i < submeshes.size(); i++)
			{
				const AABB &aabb = submeshes[i].BoundingBox;
				auto ent = s_registry->create<StaticMeshComponent, TransformComponent, BoxColliderComponent>(NodeType::Mesh);
				auto &comp = ent->getComponent<StaticMeshComponent>();
				comp.Handle = planeMsAst->getHandle();
				comp.SourceHandle = planeMs->getHandle();
				comp.SubmeshIndex = i;
				comp.Visible = true;
				comp.State.Blend = Blend::Disable();

				auto &boxComp = ent->getComponent<BoxColliderComponent>();
				// one dimension's length of plane might be zero
				boxComp.HalfSize = glm::vec3(Math::IsZero(aabb.lenX() * 0.5f) ? 1e-3 : aabb.lenX() * 0.5f,
											 Math::IsZero(aabb.lenY() * 0.5f) ? 1e-3 : aabb.lenY() * 0.5f,
											 Math::IsZero(aabb.lenZ() * 0.5f) ? 1e-3 : aabb.lenZ() * 0.5f);
				boxComp.Material = PhysicsSystem::Get()->getMaterial(PhysicsMatType::Normal);
				physicsScene->createActor<StaticMeshComponent>(ent, RigidBodyComponent::Type::Static);
			}
		}

		m_skybox = CreateRef<Skybox>("default_skybox", AssetFmt::HDR);

		EventBus::Get()->submit([]()
		{
			AssimpMeshImporter importer("werewolf", AssetFmt::GLTF);
			auto meshSource = importer.importToMeshSource();		
			EventBus::Get()->notify(EventBusArg(PioEvent::UnzipAsset, meshSource));
		});
	}

	void Scene::simulate(const Timestep &ts)
	{
		auto &comp = m_sceneRoot->getComponent<SceneComponent>();
		if (comp.Simulate)
		{
			AssetsManager::GetRuntimeAsset<PhysicsScene>(comp.PhycisScene)->simulate(ts);
		}
	}

	void Scene::onAssetUnzip(const EventBusArg &arg)
	{
		if (arg.Event == PioEvent::UnzipAsset)
		{
			if (arg.Assets)
			{
				switch (arg.Assets->getAssetType())
				{
					case AssetType::MeshSource:
					{
						Ref<MeshSource> meshSrc = RefCast<Asset, MeshSource>(arg.Assets);
						auto &sceneComp = m_sceneRoot->getComponent<SceneComponent>();
						Ref<PhysicsScene> world = AssetsManager::GetRuntimeAsset<PhysicsScene>(sceneComp.PhycisScene);
						MeshUtilParam param; 
						param.State.Blend = Blend::Disable();
						CreateMesh<BoxColliderComponent>(meshSrc, world, RigidBodyComponent::Type::Dynamic, param, m_sceneRoot);
						LOGD("mesh source[%s] is parsed", meshSrc->getName().c_str());
						break;
					}
					default:
						break;
				}
			}
			else
			{
				LOGE("parsed asset is invalid");
			}
		}
	}
}