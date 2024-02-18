#ifndef __PIONNER_PHYSICS_PHYSX_UTILS_H__
#define __PIONNER_PHYSICS_PHYSX_UTILS_H__

#include "core/Base.h"

#include <PxPhysicsAPI.h>

#define PHYSX_VEC3(vec) (physx::PxVec3(vec.x, vec.y, vec.z))

#define PHYSX_REAL(val) ((physx::PxReal)val)

#define PHYSX_RELEASE(x) if(x) { x->release(); x = nullptr; }

#define PVD_HOST "127.0.0.1"

namespace pio
{
	physx::PxShapeFlags GetShapeFlag(uint32_t flags);

	const physx::PxVec3 &ToPhysXVector(const glm::vec3 &vector);
	const physx::PxVec4 &ToPhysXVector(const glm::vec4 &vector);
	physx::PxQuat ToPhysXQuat(const glm::quat &quat);
	physx::PxMat44 ToPhysXMatrix(const glm::mat4 &matrix);

	physx::PxTransform ToPhysXTransform(const glm::vec3 &translation);
	physx::PxTransform ToPhysXTransform(const glm::vec3 &translation, const glm::quat &rotation);
}

#endif