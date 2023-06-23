#ifndef __PIONNER_WORLD_H__
#define __PIONNER_WORLD_H__

#include <unordered_map>

#include "Entity.h"

namespace Pionner
{
	class World
	{
	public:

	public:
		World();
		~World();

		void build();
		void shutdown();

		inline std::vector<std::shared_ptr<Entity>> &getEntities(EntityType type) { return m_entities[type]; }
		std::shared_ptr<Entity> getEntity(uint32_t key);

		template <class ... CompTypes>
		std::shared_ptr<Entity> createEntity(EntityType type);

		template <class ... CompTypes>
		std::shared_ptr<Entity> createEntity(EntityType type, const std::string &name);

		template<typename Func>
		void iterate(Func &&function);

	private:
		static uint32_t g_entityId;

		std::shared_ptr<decs::ECSWorld>                          m_worldImpl;
		std::vector<std::shared_ptr<Entity>>                     m_entities[ENTITY_TYPE_CNT];
		std::unordered_map<uint32_t, std::shared_ptr<Entity>>    m_entityMap;
	};

	template <class ... CompTypes>
	std::shared_ptr<Entity> World::createEntity(EntityType type)
	{
		std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(m_worldImpl, g_entityId++));
		entity->m_type = type;
		entity->m_id = m_worldImpl->new_entity<CompTypes...>();

		m_entityMap.insert(std::make_pair(entity->m_id.index, entity));
		m_entities[type].push_back(entity);
		return entity;
	}

	template <class ... CompTypes>
	std::shared_ptr<Entity> World::createEntity(EntityType type, const std::string &name)
	{
		std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(m_worldImpl, g_entityId++, name));
		entity->m_type = type;
		entity->m_id = m_worldImpl->new_entity<CompTypes...>();

		m_entityMap.insert(std::make_pair(entity->m_id.index, entity));
		m_entities[type].push_back(entity);
		return entity;
	}

	template<typename Func>
	void World::iterate(Func &&function)
	{
		m_worldImpl->for_each(std::move(function));
	}
}

#endif