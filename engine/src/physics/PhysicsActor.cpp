#include "PhysicsActor.h"
#include "PhysXInternal.h"
#include "PhysXUtils.h"

#include "scene/Registry.h"
#include "scene/Entity.h"
#include "scene/Components.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PhysicsActor"

namespace pio
{
	PhysicsActor::PhysicsActor(const Ref<Entity> &ent) : m_ent(ent)
	{
		createData();
	}

	void PhysicsActor::setActorTransform(const glm::mat4 &transform)
	{
		Ref<Entity> ent = m_ent.lock();
		PIO_ASSERT_RETURN(ent.get() != nullptr, "setActorTransform: Entity is destroied");
		m_transform = transform *
			          glm::translate(glm::mat4(1.f), ent->getComponent<RigidBodyComponent>().Center);
	}

	void PhysicsActor::setActorTransform(const glm::vec3 &translation, const glm::quat &rotate)
	{
		Ref<Entity> ent = m_ent.lock();
		PIO_ASSERT_RETURN(ent.get() != nullptr, "setActorTransform: Entity is destroied");
		m_transform = glm::translate(glm::mat4(1.f), translation) *
			          glm::toMat4(rotate) *
			          glm::translate(glm::mat4(1.f), ent->getComponent<RigidBodyComponent>().Center);
	}

	void PhysicsActor::setGlobalPose(const glm::vec3 &translation, const glm::quat &rotate)
	{
		PIO_ASSERT_RETURN(m_actor != nullptr, "setGlobalPose: actor is destroied");
		m_gTranslation = translation;
		m_gRotation = rotate;
		glm::mat4 pose = glm::translate(glm::mat4(1.f), m_gTranslation) * glm::toMat4(m_gRotation);
		m_actor->setGlobalPose(physx::PxTransform(ToPhysXMatrix(pose * m_transform)));
	}

	void PhysicsActor::setGlobalPoseDiff(const glm::vec3 &posDiff, const glm::quat &rotateDiff)
	{
		PIO_ASSERT_RETURN(m_actor != nullptr, "setGlobalPoseDiff: actor is destroied");
		//TODO: use quaternion to add rotation diff
		m_gTranslation += posDiff;		
		glm::mat4 pose = glm::translate(glm::mat4(1.f), m_gTranslation) * glm::toMat4(m_gRotation);
		m_actor->setGlobalPose(physx::PxTransform(ToPhysXMatrix(pose * m_transform)));
	}

	void PhysicsActor::createData()
	{
		Ref<Entity> ent = m_ent.lock();
		PIO_ASSERT_RETURN(ent.get() != nullptr, "createData: Entity is destroied");

		auto &comp = ent->getComponent<RigidBodyComponent>();
		m_actor = PhysXInternal::Get()->createActor(comp);
		m_actor->userData = this;

		m_transform = glm::translate(glm::mat4(1.f), comp.Center);

		if (ent->hasComponent<BoxColliderComponent>())
			addCollider<BoxColliderShape>(ent);
	}
}