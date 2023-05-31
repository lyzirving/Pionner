#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/LightComp.h"

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

		glm::vec3 lightPos{ 0.f, 3.5f, 4.f };
		std::shared_ptr<Entity> lightEntity = createEntity<LightComp>(ENTITY_POINT_LIGHT);
		auto &lightComp = lightEntity->getComp<LightComp>();

		lightComp.m_light = Light::createLight(POINT_LIGHT);
		lightComp.m_light->setPosition(lightPos);

		lightComp.m_entity = Loader::load("assets/objects/basic/bulb/bulb.obj");
		lightComp.m_entity->m_transComp.translate(lightPos.x, lightPos.y, lightPos.z);
		lightComp.m_entity->m_transComp.rotate(-90.f, 1.f, 0.f, 0.f);
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
}