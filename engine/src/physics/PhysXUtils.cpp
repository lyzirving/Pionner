#include "PhysXUtils.h"

#include "PhysicsDef.h"

namespace pio
{
	physx::PxShapeFlags GetShapeFlag(uint32_t flags)
	{
		physx::PxShapeFlags ret(0);

		if (flags & PIO_BIT(PhysicsShapeFlag::Visualize))
			ret |= physx::PxShapeFlag::eVISUALIZATION;

		if (flags & PIO_BIT(PhysicsShapeFlag::Query))
			ret |= physx::PxShapeFlag::eSCENE_QUERY_SHAPE;

		if (flags & PIO_BIT(PhysicsShapeFlag::Simulate))
			ret |= physx::PxShapeFlag::eSIMULATION_SHAPE;
		else if (flags & PIO_BIT(PhysicsShapeFlag::Trigger))
			ret |= physx::PxShapeFlag::eTRIGGER_SHAPE;

		return ret;
	}

	const physx::PxVec3 &ToPhysXVector(const glm::vec3 &vector)
	{
		return *(physx::PxVec3 *)&vector;
	}

	const physx::PxVec4 &ToPhysXVector(const glm::vec4 &vector)
	{
		return *(physx::PxVec4 *)&vector;
	}

	physx::PxQuat ToPhysXQuat(const glm::quat &quat)
	{
		return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
	}

	physx::PxMat44 ToPhysXMatrix(const glm::mat4 &matrix)
	{
		return *(physx::PxMat44 *)&matrix;
	}

	physx::PxTransform ToPhysXTransform(const glm::vec3 &translation)
	{
		return physx::PxTransform(ToPhysXVector(translation), physx::PxQuat(physx::PxIdentity));
	}

	physx::PxTransform ToPhysXTransform(const glm::vec3 &translation, const glm::quat &rotation)
	{
		return physx::PxTransform(ToPhysXVector(translation), ToPhysXQuat(rotation));
	}
}