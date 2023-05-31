#ifndef __PIONNER_WORLD_H__
#define __PIONNER_WORLD_H__

#include <unordered_map>

#include "function/framework/world/Entity.h"

namespace Pionner
{
	class World
	{
	public:
		static const std::string ENTITY_POINT_LIGHT;

	public:
		World();
		~World();

		void build();
		void shutdown();

		std::shared_ptr<Entity> getEntity(const std::string &name);

		template <class ... CompTypes>
		std::shared_ptr<Entity> createEntity();

		template <class ... CompTypes>
		std::shared_ptr<Entity> createEntity(const std::string &name);

		template<typename Func>
		void iterate(Func &&function);

	private:
		static uint32_t g_entityId;

		std::shared_ptr<decs::ECSWorld>                          m_worldImpl;
		std::unordered_map<std::string, std::shared_ptr<Entity>> m_entityMap;
	};

	template <class ... CompTypes>
	std::shared_ptr<Entity> World::createEntity()
	{
		std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(m_worldImpl, g_entityId++));
		entity->m_id = m_worldImpl->new_entity<CompTypes...>();
		m_entityMap.emplace(entity->m_name, entity);
		return entity;
	}

	template <class ... CompTypes>
	std::shared_ptr<Entity> World::createEntity(const std::string &name)
	{
		std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(m_worldImpl, g_entityId++, name));
		entity->m_id = m_worldImpl->new_entity<CompTypes...>();
		m_entityMap.emplace(entity->m_name, entity);
		return entity;
	}

	template<typename Func>
	void World::iterate(Func &&function)
	{
		m_worldImpl->for_each(std::move(function));
	}
}

#endif