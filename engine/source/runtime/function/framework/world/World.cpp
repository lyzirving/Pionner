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
		// insert default entity
		std::shared_ptr<Entity> roleEntity = createEntity<RenderComp>();
		auto &roleComp = roleEntity->getComp<RenderComp>();
		roleComp.m_entity = Loader::load("assets/objects/basic/Marry/Marry.obj");

		std::shared_ptr<Entity> bulbEntity = createEntity<RenderComp>();
		auto &bulbComp = bulbEntity->getComp<RenderComp>();
		bulbComp.m_entity = Loader::load("assets/objects/basic/bulb/bulb.obj");
		bulbComp.m_entity->m_transComp.translate(0.f, 3.f, 4.f);
		bulbComp.m_entity->m_transComp.rotate(-90.f, 1.f, 0.f, 0.f);
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