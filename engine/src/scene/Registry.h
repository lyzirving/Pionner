#ifndef __PIONNER_SCENE_REGISTRY_H__
#define __PIONNER_SCENE_REGISTRY_H__

#include "Entity.h"

namespace pio
{
	class Registry
	{
	public:
		Registry() {}
		~Registry() { shutdown(); }

		Ref<Entity> create(const std::string& name = "")
		{
			auto entity = CreateRef<Entity>(m_registry.create(), m_registry, name);
			m_entCache[entity->index()] = entity;
			return entity;
		}

		template <typename T, typename ... Comps>
		Ref<Entity> create(const std::string& name = "")
		{
			auto entity = CreateRef<Entity>(m_registry.create(), m_registry, name);
			m_entCache[entity->index()] = entity;
			entity->addComponent<T, Comps...>();
			return entity;
		}

		template<typename... Comps>
		std::vector<Ref<Entity>> view() const
		{
			std::vector<Ref<Entity>> entities;
			auto v = m_registry.view<Comps...>();
			for (auto ent : v)
			{
				auto it = m_entCache.find(PIO_ENT_KEY_2_IDX(ent));
				if (it != m_entCache.end())
				{
					entities.push_back(it->second);
				}
			}
			return entities;
		}

		void destroy(Ref<Entity>& entity)
		{
			m_entCache.erase(entity->index());
			m_registry.destroy(entity->key());
		}

		void shutdown()
		{
			auto it = m_entCache.begin();
			while (it != m_entCache.end())
			{
				m_registry.destroy(it->second->key());
				it = m_entCache.erase(it);
			}
		}

	private:
		entt::registry m_registry;
		std::unordered_map<uint32_t, Ref<Entity>> m_entCache;
	};
}

#endif