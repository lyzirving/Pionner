#ifndef __PIONNER_SCENE_ENTITY_H__
#define __PIONNER_SCENE_ENTITY_H__

#include <entt/entity/registry.hpp>

#include "Base.h"

namespace pio
{
	class Entity
	{
	public:
		Entity(const entt::entity& key, const std::string& name = "");
		~Entity() = default;

		const entt::entity& key()  const { return m_key; }
		uint32_t id() const { return m_id; }
		const std::string& name() const { return m_name; }
		const std::vector<Ref<Entity>>& children() const { return m_children; }

		void setName(const std::string& name) { m_name = name; }

		bool operator==(const Entity& rhs) { return this->m_key == rhs.m_key; }

	private:
		static uint32_t k_EntNum;

	private:
		const entt::entity m_key;
		UUID32 m_id;
		std::string m_name;
		std::vector<Ref<Entity>> m_children;
	};
}

#endif