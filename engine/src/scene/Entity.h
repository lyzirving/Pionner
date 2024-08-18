#ifndef __PIONNER_SCENE_ENTITY_H__
#define __PIONNER_SCENE_ENTITY_H__

#include <entt/entity/registry.hpp>

#include "Base.h"

namespace pio
{
	#define PIO_ENT_KEY_2_IDX(key) ((uint32_t)key)

	class Entity
	{
	public:
		Entity(const entt::entity& key, entt::registry &regi, const std::string& name = "");
		~Entity() = default;

		uint32_t index() const { return PIO_ENT_KEY_2_IDX(m_key); }
		const entt::entity& key() const { return m_key; }
		uint32_t id() const { return m_id; }

		const std::string& name() const { return m_name; }
		const std::vector<Ref<Entity>>& children() const { return m_children; }

		void setName(const std::string& name) { m_name = name; }

		bool operator==(const Entity& rhs) { return this->m_key == rhs.m_key && this->m_id == rhs.m_id; }

		template <typename T, typename ... Comps>
		void addComponent()
		{
			m_regi.emplace<T>(key());
			if constexpr (sizeof...(Comps) > 0)
			{
				addComponent<Comps...>();
			}
		}

		template <typename T, typename ... Comps>
		uint32_t removeComponent()
		{
			return m_regi.remove(T, Comps...)(key());
		}

		template <typename T>
		T* getComponent()
		{
			return m_regi.try_get<T>(m_key);
		}

	private:
		static uint32_t k_EntNum;

	private:
		const entt::entity m_key;
		entt::registry& m_regi;
		UUID32 m_id;
		std::string m_name;
		std::vector<Ref<Entity>> m_children;
	};
}

#endif