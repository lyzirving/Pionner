#ifndef __PIONNER_PHYSICS_PHYSICS_ACTOR_H__
#define __PIONNER_PHYSICS_PHYSICS_ACTOR_H__

#include "core/Base.h"

#include "PhysicsShape.h"

namespace physx
{
	class PxRigidActor;
}

namespace pio
{
	class Entity;

	class PhysicsActor
	{
	public:
		PhysicsActor(const Ref<Entity> &ent);
		~PhysicsActor() = default;
		
		void setActorTransform(const glm::vec3 &translation = glm::vec3(0.f), const glm::quat &rotate = quaternion::IDENTITY);
		void setGlobalPose(const glm::vec3 &translation = glm::vec3(0.f), const glm::quat &rotate = quaternion::IDENTITY);
		void setGlobalPoseDiff(const glm::vec3 &posDiff, const glm::quat &rotateDiff = glm::quat(0.f, 0.f, 0.f, 0.f));

	public:
		inline physx::PxRigidActor *getActor() const { return m_actor; }
		inline bool getEnt(Ref<Entity> &out) const { out = m_ent.lock(); return (out.get() != nullptr); }

	private:
		void createData();

		template<typename T>
		void addCollider(const Ref<Entity> &ent)
		{
			m_colliders.push_back(CreateRef<ColliderShape, T>(*this, ent));
		}

	private:
		WeakRef<Entity> m_ent;
		physx::PxRigidActor *m_actor{ nullptr };
		std::vector<Ref<ColliderShape>> m_colliders;

		glm::mat4 m_transform{ 1.f };

		glm::vec3 m_gTranslation{ 0.f };
		glm::quat m_gRotation{ quaternion::IDENTITY };

	private:
		friend class PhysicsScene;
	};
}

#endif