#include "function/framework/world/World.h"
#include "function/framework/comp/RenderComp.h"

#include "function/framework/load/Loader.h"

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

	void World::build()
	{
		std::shared_ptr<Entity> entity = createEntity<RenderComp>();
		auto &comp = entity->getComp<RenderComp>();
		comp.m_entity = Loader::load("assets/objects/basic/Marry/Marry.obj");
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