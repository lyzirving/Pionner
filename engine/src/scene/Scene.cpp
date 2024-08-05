#include "Scene.h"

#include "Application.h"

#include "gfx/renderer/SceneRenderer.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshBuilder.h" 
#include "gfx/struct/MaterialLibrary.h"
#include "gfx/struct/Geometry2D.h"
#include "gfx/rhi/Texture.h"

#include "core/EventBus.h"
#include "core/math/Ray.h"
#include "core/utils/Profiler.h"

#include "ui/MotionController.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Scene"

namespace pio
{
	Ref<Scene>  Scene::Main{ nullptr };
	Ref<Entity> Scene::Root{ nullptr };
	Registry *Scene::s_registry = Registry::Get();

	static void UpdateSpritePosition(const glm::vec3 &worldPos, SpriteComponent &spriteComp, Camera &cam)
	{
		const Viewport &vp = cam.viewport();
		glm::mat4 mvp = cam.prjMat() * cam.viewMat();
		glm::mat4 vpMat = Math::ViewportMat(Viewport(0, 0, vp.w(), vp.h()));
		glm::uvec2 vpSize{ vp.w(), vp.h()};

		glm::vec2 p = Math::WorldPosToScreenPt(worldPos, mvp, vpMat, vpSize);
		if (p != spriteComp.Position)
		{
			spriteComp.Position = p;
			const uint32_t w = spriteComp.ScreenWidth;
			const uint32_t h = spriteComp.ScreenHeight;
			spriteComp.Rect.Left = p.x - w / 2;
			spriteComp.Rect.Top = p.y - h / 2;
			spriteComp.Rect.Right = p.x + w / 2;
			spriteComp.Rect.Bottom = p.y + h / 2;
			Ref<QuadMesh> mesh = AssetsManager::GetRuntimeAsset<QuadMesh>(spriteComp.QuadMesh);
			mesh->Vertex.clear(); mesh->Vertex.reserve(4);
			mesh->Vertex.emplace_back(glm::vec3(Math::ScreenPtToVertex(p.x - w / 2, p.y - h / 2, vpSize.x, vpSize.y), 0.f), glm::vec2(0.f, 1.f));//lt
			mesh->Vertex.emplace_back(glm::vec3(Math::ScreenPtToVertex(p.x - w / 2, p.y + h / 2, vpSize.x, vpSize.y), 0.f), glm::vec2(0.f, 0.f));//lb
			mesh->Vertex.emplace_back(glm::vec3(Math::ScreenPtToVertex(p.x + w / 2, p.y - h / 2, vpSize.x, vpSize.y), 0.f), glm::vec2(1.f, 1.f));//rt
			mesh->Vertex.emplace_back(glm::vec3(Math::ScreenPtToVertex(p.x + w / 2, p.y + h / 2, vpSize.x, vpSize.y), 0.f), glm::vec2(1.f, 0.f));//rb	
			Renderer::SubmitTask([mesh]() mutable
			{
				mesh->VertexBuffer->setData(mesh->Vertex.data(), mesh->Vertex.size() * sizeof(QuadVertex));
			});
		}
	}

	static void LightCompToSceneData(DirectionalLightComponent &lightComp, TransformComponent &transComp, SpriteComponent &spriteComp, DirectionalLight &sceneData, Camera &cam)
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

		UpdateSpritePosition(sceneData.Position, spriteComp, cam);
	}

	static void LightCompToSceneData(PointLightComponent &comp, TransformComponent &transComp, SpriteComponent &spriteComp, PointLight &sceneData, Camera &cam)
	{		
		sceneData.Radiance = comp.Radiance;
		sceneData.Intensity = comp.Intensity;
		sceneData.MinRadius = comp.MinRadius;
		sceneData.Radius = comp.Radius;
		sceneData.Falloff = comp.Falloff;
		sceneData.SourceSize = comp.SourceSize;
		sceneData.CastShadow = comp.CastShadow;
		sceneData.Position = transComp.Transform.Position;

		Ref<StaticMesh> volume = AssetsManager::GetRuntimeAsset<StaticMesh>(sceneData.Volume);
		if (volume) { volume->getMeshSource()->as<Sphere>()->setRadius(sceneData.Radius); }

		UpdateSpritePosition(sceneData.Position, spriteComp, cam);
	}

	static void CreatePointLightComponent(uint32_t index, PointLight &light, Ref<PhysicsScene> &physicsScene, Ref<Entity> &sceneRoot, Ref<Texture2D> &icon)
	{
		Ref<Entity> ent = Registry::Get()->create<PointLightComponent, TransformComponent, RelationshipComponent, SpriteComponent>(EntityClass::PointLight, light.Name);

		auto &ptComp = ent->getComponent<PointLightComponent>();
		ptComp.Index = index;
		ptComp.Radiance = light.Radiance;
		ptComp.Intensity = light.Intensity;
		ptComp.MinRadius = light.MinRadius;
		ptComp.Radius = light.Radius;
		ptComp.Falloff = light.Falloff;
		ptComp.SourceSize = light.SourceSize;
		ptComp.CastShadow = light.CastShadow;

		auto &transComp = ent->getComponent<TransformComponent>();
		transComp.Transform.Position = light.Position;

		auto &rlComp = ent->getComponent<RelationshipComponent>();
		PIO_RELATION_SET_SELF(ent);
		PIO_RELATION_SET_PARENT(ent, sceneRoot);
		PIO_RELATION_SET_CHILD(sceneRoot, ent);

		auto &spriteComp = ent->getComponent<SpriteComponent>();
		spriteComp.Visible = true;
		spriteComp.Name = ent->getName();
		spriteComp.QuadMesh = MeshFactory::CreateScreenQuad(0, 0, 1, 1, 1, 1)->getHandle();
		spriteComp.Texture = icon->getHandle();
		spriteComp.State.DepthTest = DepthTest::Disable();
		spriteComp.State.Blend = Blend::Common();
		spriteComp.State.Cull = CullFace::Common();
		spriteComp.State.Stencil.Enable = false;
	}

	Scene::Scene(bool bMain) : EventBusObject(), m_bMain(bMain)
	{
	}

	Scene::~Scene() = default;

	void Scene::onAttach(Ref<SceneRenderer>& renderer, const LayoutParams& param)
	{		
		createData();

		m_lightEnv.DirectionalLight.obtainBlock();
		m_lightEnv.DirectionalLightShadowData.obtainBlock();
		m_lightEnv.PointLightData.obtainBlock();
		m_lightEnv.PtLightShadowData.obtainBlock();

		uint32_t w = param.Viewport.w();
		uint32_t h = param.Viewport.h();
		m_screenQuad = MeshFactory::CreateScreenQuad(0, 0, w, h, w, h)->getHandle();

		renderer->onAttach(*this, param);

		setViewport(param.Viewport.x(), param.Viewport.y(), w, h);

		EventBus::Get()->addRegister(PioEvent::UnzipAsset, EventBusCb(this, (EventBusCbFunc)&Scene::onAssetUnzip));

		EventBus::Get()->submit([]()
		{
			AssimpMeshImporter importer("werewolf", AssetFmt::GLTF);
			//AssimpMeshImporter importer("Car", AssetFmt::GLTF);
			//AssimpMeshImporter importer("fortnite", AssetFmt::GLTF);	
			//[NOTE] model 'demon' has some feature to be understood to use
			//AssimpMeshImporter importer("demon", AssetFmt::GLTF);
			auto meshSource = importer.importToMeshSource();
			EventBus::Get()->notify(EventBusArg(PioEvent::UnzipAsset, meshSource));
		});
	}

	void Scene::onDetach(Ref<SceneRenderer> &renderer)
	{
		renderer->onDetach(*this);
		EventBus::Get()->removeRegister(PioEvent::UnzipAsset, EventBusCb(this, (EventBusCbFunc)&Scene::onAssetUnzip));
	}

	void Scene::onUpdate(const Timestep &ts)
	{
		uint64_t start{ PROFILER_TIME };
		m_camera->flush();

		//[TODO] use a pixed rate for physics simulation, 
		//       maybe i need use another thread
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
				auto &lightComp = it->second->getComponent<PointLightComponent>();
				auto &transComp = it->second->getComponent<TransformComponent>();
				auto &spriteComp = it->second->getComponent<SpriteComponent>();
				LightCompToSceneData(lightComp, transComp, spriteComp, m_lightEnv.PointLightData.Lights[lightComp.Index], *m_camera.get());
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
				auto &spriteComp = ent->getComponent<SpriteComponent>();
				LightCompToSceneData(lightComp, transComp, spriteComp, m_lightEnv.DirectionalLight, *m_camera.get());
			}
			else
			{
				LOGE("Err! Mutiple distant light");
			}			
		}

		PROFILERD_DURATION(start, "Scene:Update");
	}

	void Scene::onRender(Ref<SceneRenderer> &renderer, const Timestep &ts)
	{
		uint64_t start{ PROFILER_TIME };
		// -------------------- Render 3D scene ----------------------------
		// -----------------------------------------------------------------		
		renderer->beginScene(*this, *m_camera.get());

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
				if (comp.QuadMesh == NullIndex || !comp.Visible || MotionController::bSpriteSelectd(entity))
					continue;

				renderer->submitSprite(comp.QuadMesh, comp.Texture, comp.State);
			}
		}
		PROFILERD_DURATION(start, "Scene:onRender");
		renderer->endScene(*this);
	}

	void Scene::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{		
		m_camera->setViewport(x, y, width, height);
		m_camera->setAspect(float(width) / float(height));
	}

	void Scene::createData()
	{
		// Scene and Physics
		m_sceneRoot = s_registry->create<SceneComponent, RelationshipComponent>(EntityClass::Scene, "Scene");
		PIO_RELATION_SET_SELF(m_sceneRoot);
		// NOTE: create PhysicsScene should be called at last
		m_physics = CreateRef<PhysicsScene>("Scene_Physics");
		auto &sceneComp = m_sceneRoot->getComponent<SceneComponent>();
		sceneComp.Handle = getHandle();

		m_camera = AssetsManager::CreateRuntimeAssets<Camera>();
		auto camEnt = s_registry->create<CameraComponent, RelationshipComponent>(EntityClass::Camera, "Camera");
		auto& cameraComp = camEnt->getComponent<CameraComponent>();
		PIO_RELATION_SET_SELF(camEnt);
		PIO_RELATION_SET_CHILD(m_sceneRoot, camEnt);
		PIO_RELATION_SET_PARENT(camEnt, m_sceneRoot);
		cameraComp.Primary = true;
		cameraComp.Handle = m_camera->getHandle();
		m_camera->setPosition(SphereCoord(72.f, 341.f, 10.f));
		m_camera->setLookAt(glm::vec3(0.f));
		m_camera->initSkybox("default_skybox", AssetFmt::HDR);

		if (m_bMain)
		{
			Scene::Main = self();
			Scene::Root = m_sceneRoot;
			Camera::Main = m_camera;
		}

		// Distant Light
		{
			m_lightEnv.DirectionalLight = DirectionalLight(glm::vec3(-4.5f, 3.8f, -1.f), glm::vec3(0.f), glm::vec3(1.f), 1.f);

			Ref<Entity> ent = Registry::Get()->create<DirectionalLightComponent, RelationshipComponent, SpriteComponent, TransformComponent>(EntityClass::DirectionalLight, "DirectionalLight");

			auto &transComp = ent->getComponent<TransformComponent>();
			auto &lightComp = ent->getComponent<DirectionalLightComponent>();
			transComp.Transform.Position = m_lightEnv.DirectionalLight.Position;
			lightComp.Direction = m_lightEnv.DirectionalLight.Direction;
			lightComp.Radiance = m_lightEnv.DirectionalLight.Radiance;
			lightComp.Intensity = m_lightEnv.DirectionalLight.Intensity;

			auto &rlComp = ent->getComponent<RelationshipComponent>();
			PIO_RELATION_SET_SELF(ent);
			PIO_RELATION_SET_PARENT(ent, m_sceneRoot);
			PIO_RELATION_SET_CHILD(m_sceneRoot, ent);

			TextureSpecification spec; 
			spec.SRGB = true;
			spec.FlipVerticalWhenLoad = true;
			Ref<Texture2D> icon = Texture2D::Create(AssetsManager::SpriteAbsPath("distant_light", AssetFmt::PNG), spec);
			AssetsManager::Get()->addRuntimeAsset(icon);
			Renderer::SubmitTask([icon]() mutable { icon->init(); });

			auto &spriteComp = ent->getComponent<SpriteComponent>();
			spriteComp.Visible = true;
			spriteComp.Name = ent->getName();
			spriteComp.QuadMesh = MeshFactory::CreateScreenQuad(0, 0, 1, 1, 1, 1)->getHandle(); 
			spriteComp.Texture = icon->getHandle();
			spriteComp.State = RenderState(Blend::Common(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable());
		}

		//Point Light
		{			
			TextureSpecification spec;
			spec.SRGB = true;
			spec.FlipVerticalWhenLoad = true;
			Ref<Texture2D> icon = Texture2D::Create(AssetsManager::SpriteAbsPath("point_light", AssetFmt::PNG), spec);
			AssetsManager::Get()->addRuntimeAsset(icon);
			Renderer::SubmitTask([icon]() mutable { icon->init(); });

			m_lightEnv.PointLightData.LightCount = 1;
			m_lightEnv.PtLightShadowData.LightCount = m_lightEnv.PointLightData.LightCount;

			m_lightEnv.PointLightData.Lights[0] = PointLight(SphereCoord::ToCCS(52.f, 45.f, 2.f),
															 glm::vec3(1.f), 1.f, 0.001f, 2.25f, 0.5f, 0.1f, "PointLight0");
			m_lightEnv.PointLightData.Lights[0].Volume = AssetsManager::CreateRuntimeAssets<StaticMesh>(MeshFactory::CreateSphere(m_lightEnv.PointLightData.Lights[0].Radius))->getHandle();
			m_lightEnv.PointLightData.Lights[1] = PointLight(SphereCoord::ToCCS(40.f, 290.f, 3.2f),
															 glm::vec3(1.f), 1.f, 0.001f, 2.25f, 0.5f, 0.1f, "PointLight1");
			m_lightEnv.PointLightData.Lights[1].Volume = AssetsManager::CreateRuntimeAssets<StaticMesh>(MeshFactory::CreateSphere(m_lightEnv.PointLightData.Lights[1].Radius))->getHandle();
			for (uint32_t i = 0; i < m_lightEnv.PointLightData.LightCount; i++)
				CreatePointLightComponent(i, m_lightEnv.PointLightData.Lights[i], m_physics, m_sceneRoot, icon);
		}

		// Plane
		{			
			MeshBuildParam param;
			param.meshSrc = MeshFactory::CreatePlane();
			param.physicWorld = m_physics;
			param.State = RenderState(Blend::Disable(), DepthTest::Common(), CullFace::Common(), StencilTest::Disable());
			param.RigidType = RigidBodyComponent::Type::Static;
			param.Parent = m_sceneRoot;
			CreateStaticMesh<BoxColliderComponent>(param);
		}
	}

	void Scene::simulate(const Timestep &ts)
	{
		if (m_bSimulate)
		{
			m_physics->simulate(ts);
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
						MeshBuildParam param; 
						param.meshSrc = meshSrc;
						param.physicWorld = m_physics;
						param.State = RenderState(Blend::Disable(), DepthTest::Common(), CullFace::Common(), StencilTest::Disable());
						param.RigidType = RigidBodyComponent::Type::Dynamic;
						param.Parent = m_sceneRoot;
						auto asset = CreateDynamicMesh<BoxColliderComponent>(param);
						LOGD("mesh source[%s] is parsed, uid[%u]", meshSrc->getName().c_str(), (uint32_t)asset->getHandle());
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

	template<>
	bool Asset::is<Scene>() const { return getAssetType() == AssetType::Scene; }
}