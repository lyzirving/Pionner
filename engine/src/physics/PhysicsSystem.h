#ifndef __PIONNER_PHYSICS_PHYSICS_SYSTEM_H__
#define __PIONNER_PHYSICS_PHYSICS_SYSTEM_H__

#include "PhysicsDef.h"
#include "asset/Asset.h"

namespace pio
{
	class PhysicsSystem
	{
		PIO_SINGLETON_DECLARE(PhysicsSystem)
	public:
		inline AssetHandle getMaterial(PhysicsMatType type) const { return m_materials[(uint8_t)type]; }

	private:
		void createMaterial();

	private:
		AssetHandle m_materials[(uint8_t)PhysicsMatType::Num];
	};
}

#endif