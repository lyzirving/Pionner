#include "function/framework/world/World.h"
#include "function/framework/comp/RenderComp.h"

#include "function/framework/load/Loader.h"

#include "function/framework/world/light/PointLight.h"

#include "core/log/LogSystem.h"

using namespace decs;

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "World"

namespace Pionner
{
	uint32_t World::g_entityId = 0;
	const std::string World::ENTITY_POINT_LIGHT = "PointLight";

	World::World() : m_worldImpl(new decs::ECSWorld), m_entityMap(), m_lights()
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

		std::shared_ptr<Entity> bulbEntity = createEntity<RenderComp>(ENTITY_POINT_LIGHT);
		auto &bulbComp = bulbEntity->getComp<RenderComp>();
		bulbComp.m_entity = Loader::load("assets/objects/basic/bulb/bulb.obj");
		bulbComp.m_entity->m_transComp.translate(0.f, 3.f, 4.f);
		bulbComp.m_entity->m_transComp.rotate(-90.f, 1.f, 0.f, 0.f);

		std::shared_ptr<Light> pointLight = std::shared_ptr<Light>(new PointLight(bulbComp.m_entity->m_transComp.getPosition()));
		m_lights.insert(std::make_pair(ENTITY_POINT_LIGHT, pointLight));
	}

	std::shared_ptr<Entity> World::getEntity(const std::string &name)
	{
		auto itr = m_entityMap.find(name);
		if (itr != m_entityMap.end())
		{
			auto ret = itr->second;
			return ret;
		}
		else
		{
			return std::shared_ptr<Entity>();
		}
	}

	std::shared_ptr<Light> World::getLight(const std::string &name)
	{
		auto itr = m_lights.find(name);
		if (itr != m_lights.end())
		{
			auto ret = itr->second;
			return ret;
		}
		else
		{
			return std::shared_ptr<Light>();
		}
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

		auto itr1 = m_lights.begin();
		while (itr1 != m_lights.end())
		{
			itr1->second.reset();
			itr1 = m_lights.erase(itr1);
		}
	}
}