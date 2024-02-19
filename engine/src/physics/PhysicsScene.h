#ifndef __PIONNER_PHYSICS_PHYSICS_SCENE_H__
#define __PIONNER_PHYSICS_PHYSICS_SCENE_H__

#include "PhysicsDef.h"

#include "scene/Components.h"

namespace physx
{
	class PxScene;
	class PxRigidActor;
}

namespace pio
{
	class Ray;
	class Entity;

	class PhysicsScene : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::PhysicsScene)
	public:
		PhysicsScene(const std::string &name);
		~PhysicsScene();

		HitResult intersect(const Ray &r);
		void simulate(const Timestep &ts);

	public:
		template<typename TargetComp>
		Ref<PhysicsActor> createActor(const Ref<Entity> &ent, RigidBodyComponent::Type type)
		{
			if (!ent)
				return Ref<PhysicsActor>();

			if (!ent->any<BoxColliderComponent, SphereColliderComponent, MeshColliderComponent, CapsuleColliderComponent>())
				return Ref<PhysicsActor>();

			if (!ent->hasComponent<RigidBodyComponent>())
				ent->addComponent<RigidBodyComponent>();

			makeRigidBody<TargetComp>(ent, type);

			return createPhysicsActor(ent);
		}

	private:
		Ref<PhysicsActor> createPhysicsActor(const Ref<Entity> &ent);

	private:
		template<typename Comp>
		void makeRigidBody(const Ref<Entity> &ent, RigidBodyComponent::Type type)
		{
			// none implementation by default, specific component has their unique implementation.
		}
		
	private:
		physx::PxScene *m_scene{ nullptr };
		
		std::unordered_map<UUID32, Ref<PhysicsActor>> m_actors;
	};

	template<>
	void PhysicsScene::makeRigidBody<StaticMeshComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type);

	template<>
	void PhysicsScene::makeRigidBody<MeshComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type);

	template<>
	void PhysicsScene::makeRigidBody<C3dUIComponent>(const Ref<Entity> &ent, RigidBodyComponent::Type type);
}

#endif