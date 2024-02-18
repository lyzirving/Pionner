#ifndef __PIONNER_SCENE_REGISTRY_H__
#define __PIONNER_SCENE_REGISTRY_H__

#include <map>
#include <initializer_list>
#include "Entity.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Registry"

namespace pio
{
	/*
	* Entity Registry for ecs system.
	*/
	class Registry
	{
		PIO_SINGLETON_DECLARE(Registry);
	public:
		Ref<Entity> mainCameraEnt();
		Ref<Entity> mainSceneEnt();
		void destroy(Entity &entity);

	public:
		template <typename ... Comp>
		Ref<Entity> create(NodeType type = NodeType::None)
		{
			decs::EntityID index = m_ECSImpl.new_entity();
			Ref<Entity> entity = CreateRef<Entity>(index, type);
			uint32_t cnt = sizeof...(Comp);
			if (cnt != 0)
			{
				(void)std::initializer_list<int>{ (entity->addComponent<Comp>(), 0)... };
			}
			cacheEntity(entity);
			return entity;
		}

		template <typename Comp>
		void emplace(Entity &entity)
		{
			if (!m_ECSImpl.has_component<Comp>(entity.m_handle))
			{
				m_ECSImpl.add_component_default<Comp>(entity.m_handle);
			}
			else
			{
				LOGW("already has component");
			}
		}

		template <typename Comp, typename P, typename...Args>
		void emplace(Entity &entity, P &&p, Args...args)
		{
			if (!m_ECSImpl.has_component<Comp>(entity.m_handle))	
			{
				m_ECSImpl.add_component<Comp, P, Args...>(entity.m_handle, std::forward<P>(p), std::forward<Args>(args)...);
			}
			else
			{
				LOGW("already has component");
			}
		}

		template <typename Comp>
		Comp &getSingleton()
		{
			Comp *p = m_ECSImpl.get_singleton<Comp>();
			if (!p)
			{
				p = m_ECSImpl.set_singleton<Comp>();
			}
			return *p;
		}

		template <typename Comp>
		bool hasComponent(Entity &entity)
		{
			return m_ECSImpl.has_component<Comp>(entity.m_handle);
		}

		template <typename Comp>
		Comp &getComponent(Entity &entity)
		{
			return m_ECSImpl.get_component<Comp>(entity.m_handle);
		}

		template <typename Comp>
		void removeComponent(Entity &entity)
		{
			m_ECSImpl.remove_component<Comp>(entity.m_handle);
		}

		template <typename Comp>
		EntityView view()
		{
			EntityView view;
			m_ECSImpl.for_each([&](decs::EntityID id, Comp comp)
			{
				Ref<Entity> entity = getCachedEntity(id.index);
				if (entity) { view.add(entity); }
			});
			return view;
		}

		template <typename CompA, typename CompB>
		EntityView view()
		{
			EntityView view;
			m_ECSImpl.for_each([&](decs::EntityID id, CompA compA, CompB compB)
			{
				Ref<Entity> entity = getCachedEntity(id.index);
				if (entity) { view.add(entity); }
			});
			return view;
		}

		template <typename CompA, typename CompB, typename CompC>
		EntityView view()
		{
			EntityView view;
			m_ECSImpl.for_each([&](decs::EntityID id, CompA compA, CompB compB, CompC compC)
			{
				Ref<Entity> entity = getCachedEntity(id.index);
				if (entity) { view.add(entity); }
			});
			return view;
		}

		inline Ref<Entity> getCachedEntity(uint32_t index)
		{
			if (m_entities.size() > index) { return m_entities[index]; }
			LOGE("index[%u] out of bounds[%u]", index, m_entities.size());
			return Ref<Entity>();
		}
	private:
		void cacheEntity(const Ref<Entity> &entity);

	private:
		decs::ECSWorld m_ECSImpl{};
		std::vector<Ref<Entity>> m_entities{};
	};
}

#endif