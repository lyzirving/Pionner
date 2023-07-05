#include "World.h"

#include "world/comp/RenderComp.h"
#include "world/comp/LightComp.h"
#include "world/comp/GeometryComp.h"
#include "world/comp/ShadowComp.h"

#include "global/comp/TransformComponent.h"
#include "global/comp/MaterialComponent.h"

#include "core/system/Loader.h"

#include "render/entity/ModelEntity.h"

#include "core/log/LogSystem.h"

using namespace decs;

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "World"

namespace pio
{
	uint32_t World::g_entityId = 0;

	World::World() : m_worldImpl(new decs::ECSWorld), m_entities(), m_entityMap()
	{
	}

	World::~World()
	{
		shutdown();
		m_worldImpl.reset();
	}

	void World::build()
	{
		// Insert default entity
		auto role0 = createEntity<RenderComp, ShadowComp>(ENTITY_OBJ, "role0");
		auto &role0Comp = role0->getComp<RenderComp>();
		role0Comp.m_entity = std::shared_ptr<RenderEntity>(new ModelEntity);
		Loader::load("assets/objects/spiderman/spiderman.obj", role0Comp.m_entity);
		role0Comp.m_entity->m_transComp.translate(0.f, 0.1f, 0.f);

		/*auto role1 = createEntity<RenderComp, ShadowComp>(ENTITY_OBJ, "role1");
		auto &role1Comp = role1->getComp<RenderComp>();
		role1Comp.m_entity = std::shared_ptr<RenderEntity>(new ModelEntity);
		Loader::load("assets/objects/nanosuit/nanosuit.obj", role1Comp.m_entity);
		role1Comp.m_entity->m_transComp.translate(3.f, 0.1f, 2.5f);
		role1Comp.m_entity->m_transComp.rotate(-30.f, 0.f, 1.f, 0.f);*/

		auto lightEntity = createEntity<LightComp>(ENTITY_LIGHT, "light");
		auto &lightComp = lightEntity->getComp<LightComp>();
		lightComp.m_pos = glm::vec3(-1.5f, 4.f, 2.5f);
		lightComp.m_dir = glm::vec3(0.f) - lightComp.m_pos;
		//lightComp.m_type = LIGHT_TYPE_DIRECTIONAL;
		lightComp.m_type = LIGHT_TYPE_POINT;
		lightComp.m_obj = std::shared_ptr<RenderEntity>(new ModelEntity(RENDER_MODE_MATERIAL_DISPLAY));
		Loader::load("assets/objects/bulb/bulb.obj", lightComp.m_obj);
		lightComp.m_obj->m_transComp.translate(lightComp.m_pos);
		lightComp.m_obj->m_transComp.rotate(180.f, 1.f, 0.f, 0.f);

		// Add a plane
		auto planeEntity = createEntity<GeometryComp, ShadowComp>(ENTITY_GEOMETRY, "plane");
		auto &geoComp = planeEntity->getComp<GeometryComp>();
		geoComp.m_geometry = Geometry::createGeometry(GEO_TYPE_PLANE);
		auto planeTrans = geoComp.m_geometry->getTransformComp();
		planeTrans->translate(0.f, 0.1f, 0.f);
		planeTrans->scale(6.f, 6.f, 6.f);
		auto planeMesh = geoComp.m_geometry->getMeshComp();
		planeMesh->m_color = glm::vec4(0.73f, 0.73f, 0.73f, 1.f);
	}

	void World::shutdown()
	{
		for (uint8_t i = 0; i < ENTITY_TYPE_CNT; i++)
		{
			auto itr = m_entities[i].begin();
			while (itr != m_entities[i].end())
			{
				m_worldImpl->destroy((*itr)->m_id);
				(*itr).reset();
				itr = m_entities[i].erase(itr);
			}
		}

		auto itr = m_entityMap.begin();
		while (itr != m_entityMap.end())
		{
			itr->second.reset();
			itr = m_entityMap.erase(itr);
		}
	}

	std::shared_ptr<Entity> World::getEntity(uint32_t key)
	{
		std::shared_ptr<Entity> result{ nullptr };
		auto itr = m_entityMap.find(key);
		if (itr != m_entityMap.end())
		{
			result = itr->second;
		}
		return result;
	}
}