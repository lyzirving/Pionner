#include "PhysicsShape.h"
#include "PhysicsActor.h"
#include "PhysXUtils.h"
#include "PhysXInternal.h"
#include "PhysicsMaterial.h"

#include "scene/Components.h"
#include "scene/Registry.h"
#include "scene/Entity.h"

#include "asset/AssetsManager.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ColliderShape"

namespace pio
{
	void ColliderShape::release()
	{
		PHYSX_RELEASE(m_material);
		PHYSX_RELEASE(m_shape);
	}

	BoxColliderShape::BoxColliderShape(const PhysicsActor &actor, const Ref<Entity> &ent)
		: ColliderShape(ent)
	{
		if (!m_ent->hasComponent<BoxColliderComponent>())
		{
			LOGE("err! entity must have BoxColliderComponent");
			return;
		}
		auto &comp = m_ent->getComponent<BoxColliderComponent>();
		m_halfSize = comp.HalfSize;
		m_flags = comp.ShapeFlags;
		Ref<PhysicsMaterial> mat = AssetsManager::GetRuntimeAsset<PhysicsMaterial>(comp.Material);
		m_shape = PhysXInternal::Get()->createShape(this, mat.get());
		if (m_shape)
		{
			actor.getActor()->attachShape(*m_shape);
			PHYSX_RELEASE(m_shape);
		}
		else
		{
			LOGE("fail to create shape for BoxColliderShape");
		}
	}

	void BoxColliderShape::detachFromActor(physx::PxRigidActor *actor)
	{
		if (actor && m_shape)
			actor->detachShape(*m_shape);
	}

	template<>
	bool ColliderShape::is<BoxColliderShape>() const { return getShapeType() == ColliderType::Box; }
}