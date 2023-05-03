#include "function/framework/world/World.h"

#include "core/log/LogSystem.h"

using namespace decs;

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "World"

namespace Pionner
{
	uint32_t World::g_entityId = 0;

	World::World() : m_worldImpl(new decs::ECSWorld), m_entityMap()
	{
	}

	World::~World()
	{
		shutdown();
		m_worldImpl.reset();
	}

	void World::initialize()
	{

	}

	void World::shutdown()
	{
		auto itr = m_entityMap.begin();
		while (itr != m_entityMap.end())
		{
			m_worldImpl->destroy(itr->second->m_id);
			itr->second.reset();
			itr = m_entityMap.erase(itr);
		}
	}
}