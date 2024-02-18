#ifndef __PIONNER_PHYSICS_PHYSX_INTERNAL_H__
#define __PIONNER_PHYSICS_PHYSX_INTERNAL_H__

#include "PhysicsDef.h"

namespace physx
{
	class PxScene;
	class PxRigidActor;
	class PxMaterial;
	class PxShape;
}

namespace pio
{
	struct RigidBodyComponent;
	class PhysicsMaterial;
	class ColliderShape;

	class PhysXInternal
	{
		PIO_SINGLETON_DECLARE(PhysXInternal)
	public:
		physx::PxScene *createScene(const PhysicsSceneDesc& spec);
		physx::PxRigidActor *createActor(const RigidBodyComponent &comp);
		physx::PxMaterial *createMaterial(const Ref<PhysicsMaterial> &mat);
		physx::PxShape *createShape(ColliderShape *shape, PhysicsMaterial *mat);
	};
}

#endif