#include "World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/LightComp.h"
#include "function/framework/comp/GeometryComp.h"
#include "function/framework/comp/OcclusionComp.h"

#include "function/framework/load/Loader.h"

#include "function/render/entity/ModelEntity.h"
#include "function/render/entity/LightEntity.h"

#include "core/log/LogSystem.h"

using namespace decs;

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "World"

namespace Pionner
{
	uint32_t World::g_entityId = 0;

	World::World() : m_worldImpl(new decs::ECSWorld), m_entities()
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
		auto roleEntity = createEntity<RenderComp, OcclusionComp>(ENTITY_OBJ, "role");
		auto &roleComp = roleEntity->getComp<RenderComp>();
		roleComp.m_entity = std::shared_ptr<RenderEntity>(new ModelEntity);
		Loader::load("assets/objects/basic/Marry/Marry.obj", roleComp.m_entity);
		roleComp.m_entity->m_transComp.translate(0.f, 0.1f, 0.f);

		auto lightEntity = createEntity<LightComp>(ENTITY_LIGHT, "light");
		auto &lightComp = lightEntity->getComp<LightComp>();
		lightComp.m_type = LIGHT_TYPE_DIRECTIONAL;
		// Note the directional light points at world center
		lightComp.m_pos = glm::vec3(-3.f, 5.f, 4.f);
		lightComp.m_dir = glm::vec3(0.f) - lightComp.m_pos;

		// Add a plane
		auto planeEntity = createEntity<GeometryComp>(ENTITY_GEOMETRY, "plane");
		auto &geoComp = planeEntity->getComp<GeometryComp>();
		geoComp.m_geometry = Geometry::createGeometry(GEO_TYPE_PLANE);
		auto planeTrans = geoComp.m_geometry->getTransformComp();
		planeTrans->translate(0.f, 0.1f, 0.f);
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
	}
}