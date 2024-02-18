#include "PhysicsSystem.h"
#include "PhysXInternal.h"
#include "PhysicsMaterial.h"

#include "asset/AssetsManager.h"

namespace pio
{
	PIO_SINGLETON_IMPL(PhysicsSystem)

	PhysicsSystem::PhysicsSystem()
	{
		for (uint8_t i = 0; i < (uint8_t)PhysicsMatType::Num; i++)
			m_materials[i] = NullAsset;
	}

	PhysicsSystem::~PhysicsSystem() = default;

	void PhysicsSystem::Init()
	{
		PhysicsSystem::Get();
		PhysXInternal::Init();
		PhysicsSystem::Get()->createMaterial();
	}

	void PhysicsSystem::Shutdown()
	{
		PhysXInternal::Shutdown();
		{
			std::lock_guard<std::mutex> lk{ s_mutex };
			delete s_instance;
			s_instance = nullptr;
		}
	}

	void PhysicsSystem::createMaterial()
	{
		for (uint8_t i = 0; i < (uint8_t)PhysicsMatType::Num; i++)
		{
			switch (PhysicsMatType(i))
			{
				case PhysicsMatType::Normal:
				{
					if (m_materials[i] != NullAsset)
						break;

					Ref<PhysicsMaterial> mat = CreateRef<PhysicsMaterial>(PhysicsMatType::Normal);
					mat->StaticFriction = 98.f;
					mat->DynamicFriction = 100.f;
					mat->Bounciness = 0.01;
					mat->m_material = PhysXInternal::Get()->createMaterial(mat);
					m_materials[i] = mat->getHandle();
					AssetsManager::Get()->addRuntimeAsset(mat);
					break;
				}
				default:
					break;
			}
		}
	}
}