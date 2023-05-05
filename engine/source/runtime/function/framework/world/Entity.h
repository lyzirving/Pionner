#ifndef __PIONNER_ENTITY_H__
#define __PIONNER_ENTITY_H__

#include <string>
#include <vector>
#include <memory>

#include <decs.h>

#include "function/framework/comp/Comp.h"

namespace Pionner
{
	class Entity
	{
		friend class World;

	public:
		virtual ~Entity();

		template <class T>
		void addComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type &comp);

		template <class T>
		T &getComp();

		template <class T>
		bool hasComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type *placeholder = nullptr);

		template <class T>
		void removeComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type *placeholder = nullptr);

	private:
		Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id);
		Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id, const std::string &name);

	public:
		const std::string m_name;
		decs::EntityID    m_id;

	private:
		std::shared_ptr<decs::ECSWorld> m_worldImpl;
	};

	template <class T>
	void Entity::addComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type &comp)
	{
		m_worldImpl->add_component<T>(m_id, comp);
	}

	template <class T>
	T &Entity::getComp()
	{
		return m_worldImpl->get_component<T>(m_id);
	}

	template <class T>
	bool Entity::hasComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type *)
	{
		return m_worldImpl->has_component<T>(m_id);
	}

	template <class T>
	void Entity::removeComp(typename std::enable_if<std::is_base_of<Comp, T>::value, T>::type *)
	{
		m_worldImpl->remove_component<T>(m_id);
	}
}

#endif