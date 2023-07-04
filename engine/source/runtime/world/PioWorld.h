#ifndef __PIONNER_PIO_WORLD_H__
#define __PIONNER_PIO_WORLD_H__

#include <unordered_map>
#include <string>
#include <memory>

#include "entity/PioEntity.h"

namespace pio
{
	class PioWorld
	{
	public:
		PioWorld();
		~PioWorld();

		template <class ... CompTypes>
		std::shared_ptr<PioEntity> createEntity();

		void init();
		void shutdown();

	private:
		static uint32_t g_entityId;
		typedef std::unordered_map<std::string, std::shared_ptr<PioEntity>> Collection;

		decs::ECSWorld m_ecsWorld;
		Collection     m_entities;
	};

	template <class ... CompTypes>
	std::shared_ptr<PioEntity> PioWorld::createEntity()
	{
		std::string key = "entity_" + std::to_string(g_entityId);
		auto itr = m_entities.find(key);
		if (itr != m_entities.end())
		{
			// already exist
			return itr->second;
		}
		auto entity = std::shared_ptr<PioEntity>(new PioEntity);
		entity->m_id = g_entityId;
		entity->m_key = key;
		if (entity->createComps<CompTypes ...>())
		{
			entity->m_ecsId = m_ecsWorld.new_entity<CompTypes...>();
			g_entityId++;
			m_entities.insert(std::make_pair(entity->m_key, entity));
			return entity;
		}
		// failed to createComps()
		entity.reset();
		return entity;
	}
}

#endif