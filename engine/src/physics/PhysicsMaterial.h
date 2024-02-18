#ifndef __PIONNER_PHYSICS_PHYSICS_MATERIAL_H__
#define __PIONNER_PHYSICS_PHYSICS_MATERIAL_H__

#include "PhysicsDef.h"

#include "asset/Asset.h"

namespace physx
{
	class PxMaterial;
}

namespace pio
{
	class PhysicsMaterial : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::PhysicsMat)
	public:
		PhysicsMaterial(PhysicsMatType type);
		virtual ~PhysicsMaterial();

		inline PhysicsMatType getType() const { return m_type; }
		inline physx::PxMaterial *getMaterial() const { return m_material; }

	public:
		// If static friction is larger, it is more difficult to make the rigid body begin to move in static state.
		float StaticFriction{ 0.f };
		// Coefficient of friction when rigid body is moving.
		float DynamicFriction{ 0.f };
		// It's in [0, 1]. if bounciness is 1.f, the rigid body in free fall will bounce back to the same height 
		// when it contacts with floor.
		float Bounciness{ 0.f };

	private:
		PhysicsMatType m_type;
		physx::PxMaterial *m_material{ nullptr };

	private:
		friend class PhysXInternal;
		friend class PhysicsSystem;
	};
}

#endif