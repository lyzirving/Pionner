#include "PhysicsScene.h"

#include "PhysXInternal.h"
#include "PhysicsActor.h"
#include "PhysXUtils.h"

#include "scene/Registry.h"
#include "scene/Entity.h"

#include "gfx/struct/Mesh.h"

#include "ui/UiDef.h"
#include "core/math/Ray.h"
#include "asset/AssetsManager.h"

#include <PxScene.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PhysicsScene"

namespace pio
{
	PhysicsScene::PhysicsScene(const std::string &name) : Asset(name)
	{
		PhysicsSceneDesc spec;
		m_scene = PhysXInternal::Get()->createScene(spec);
	}

	PhysicsScene::~PhysicsScene()
	{
		PHYSX_RELEASE(m_scene);
	}

	HitResult PhysicsScene::intersect(const Ray &r)
	{
		float hitLimit = 100.f;
		physx::PxRaycastBuffer hitCall;
		m_scene->raycast(PHYSX_VEC3(r.Origin), PHYSX_VEC3(r.Dir), PHYSX_REAL(hitLimit), hitCall);
		uint32_t nb = hitCall.getNbAnyHits();

		HitResult result;
		result.Hit = nb > 0;
		result.HitCnt = nb;
		result.Actor = result.Hit ? static_cast<PhysicsActor *>(hitCall.getAnyHit(0).actor->userData) : nullptr;
		return result;
	}

	void PhysicsScene::simulate(const Timestep &ts)
	{
		if (!m_scene)
			return;

		// TODO: Make large elapsedTime values subdivided into smaller time intervals
		m_scene->simulate(ts.getSeconds());
		m_scene->fetchResults(true);
	}

	Ref<PhysicsActor> PhysicsScene::createPhysicsActor(const Ref<Entity> &ent)
	{
		auto it = m_actors.find(ent->getUid());
		if (it != m_actors.end())
			return it->second;

		if (!ent->hasComponent<RigidBodyComponent>())
		{
			LOGE("err, entity does not have RigidBodyComponent");
			return Ref<PhysicsActor>();
		}

		auto actor = CreateRef<PhysicsActor>(ent);
		if (actor->m_actor)
		{
			LOGD("create physics actor[%s]", actor->m_actor->getName());
			m_actors[ent->getUid()] = actor;
			ent->attachActor(actor);
			m_scene->addActor(*actor->m_actor);
			// Set default transform matrix caused by RigibodyComponent
			actor->setActorTransform();
		}
		else
		{
			LOGE("fail to create physics actor for entity[%u]", (uint32_t)ent->getUid());
		}
		return actor;
	}

	template<>
	void PhysicsScene::makeRigidBody<StaticMeshComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type)
	{
		auto &smComp = ent->getComponent<StaticMeshComponent>();
		Ref<MeshSource> source = AssetsManager::GetRuntimeAsset<StaticMesh>(smComp.Handle)->getMeshSource();

		auto &rbComp = ent->getComponent<RigidBodyComponent>();
		rbComp.BodyType = type;
		rbComp.Center = source->getAABB().center();
		rbComp.Name = source->getName();
	}

	template<>
	void PhysicsScene::makeRigidBody<MeshComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type)
	{
		auto &msComp = ent->getComponent<MeshComponent>();
		Ref<MeshSource> source = AssetsManager::GetRuntimeAsset<Mesh>(msComp.Handle)->getMeshSource();
		const Submesh &subMs = source->getSubmeshes()[msComp.SubmeshIndex];

		auto &rbComp = ent->getComponent<RigidBodyComponent>();
		rbComp.BodyType = type;
		rbComp.Center = subMs.Transform * glm::vec4(glm::vec3(subMs.BoundingBox.center()), 1.f);
		rbComp.Name = subMs.MeshName;
	}

	template<>
	void PhysicsScene::makeRigidBody<Ui3DComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type)
	{
		auto &smComp = ent->getComponent<Ui3DComponent>();
		Ref<MeshSource> source = AssetsManager::GetRuntimeAsset<StaticMesh>(smComp.Handle)->getMeshSource();

		auto &rbComp = ent->getComponent<RigidBodyComponent>();
		rbComp.BodyType = type;
		rbComp.Center = source->getAABB().center();
		rbComp.Name = source->getName();
	}
}