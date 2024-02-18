#include "PhysicsMaterial.h"
#include "PhysXUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PhysicsMaterial"

namespace pio
{
	PhysicsMaterial::PhysicsMaterial(PhysicsMatType type) : Asset(), m_type(type)
	{
	}

	PhysicsMaterial::~PhysicsMaterial()
	{
		PHYSX_RELEASE(m_material);
	}
}