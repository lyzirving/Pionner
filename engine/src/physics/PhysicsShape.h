#ifndef __PIONNER_PHYSICS_PHYSICS_SHAPE_H__
#define __PIONNER_PHYSICS_PHYSICS_SHAPE_H__

#include "PhysicsDef.h"

namespace physx
{
	class PxShape;
	class PxMaterial;
	class PxRigidActor;
	class PxShape;
}

namespace pio
{
	class Entity;
	class PhysicsActor;

	class ColliderShape
	{
	public:
		ColliderShape(const Ref<Entity> &ent) : m_ent(ent) {}
		virtual ~ColliderShape() = default;

		virtual void release();

		virtual const char *getShapeName() const = 0;
		virtual ColliderType getShapeType() const = 0;
		virtual void detachFromActor(physx::PxRigidActor *actor) = 0;

	public:
		template<typename T>
		bool is() const { return false; }

		template<typename T>
		T *as() { if (std::is_base_of<ColliderShape, T>() && is<T>()) { return static_cast<T *>(this); } else { return nullptr; } }

	protected:
		Ref<Entity> m_ent;
		physx::PxMaterial *m_material{nullptr};
		physx::PxShape *m_shape{ nullptr };
		uint32_t m_flags{ 0 };

	private:
		friend class PhysXInternal;
	};

	class BoxColliderShape : public ColliderShape
	{
		PIO_COLLIDER_IMPL(ColliderType::Box)

	public:
		BoxColliderShape(const PhysicsActor &actor, const Ref<Entity> &ent);
		virtual ~BoxColliderShape() = default;

		virtual void detachFromActor(physx::PxRigidActor *actor) override;
	private:
		glm::vec3 m_halfSize{ 0.f };

	private:
		friend class PhysXInternal;
	};

	template<>
	bool ColliderShape::is<BoxColliderShape>() const;
}

#endif