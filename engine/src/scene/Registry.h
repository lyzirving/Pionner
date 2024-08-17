#ifndef __PIONNER_SCENE_REGISTRY_H__
#define __PIONNER_SCENE_REGISTRY_H__

#include "Entity.h"

namespace pio
{
	class Registry
	{
	public:
		static void Init();
		static void Shutdown();

		static Ref<Entity> Create(const std::string& name = "")
		{
			auto entity = CreateRef<Entity>(k_Registry.create(), name);
			k_EntCache[entity->id()] = entity;
			return entity;
		}

		template <typename T, typename ... Comps>
		static Ref<Entity> Create(const std::string& name = "")
		{
			auto entity = CreateRef<Entity>(k_Registry.create(), name);
			k_EntCache[entity->id()] = entity;
			AddComponent<T, Comps...>(entity);
			return entity;
		}

		template <typename T, typename ... Comps>
		static void AddComponent(Ref<Entity>& entity)
		{
			k_Registry.emplace<T>(entity->key());
			if constexpr (sizeof...(Comps) > 0) { AddComponent<Comps...>(entity); }
		}

		template <typename T, typename ... Comps>
		static uint32_t RemoveComponent(Ref<Entity>& entity)
		{
			return k_Registry.remove(T, Comps...)(entity->key());s
		}

		static void Destroy(Ref<Entity>& entity)
		{
			k_EntCache.erase(entity->id());
			k_Registry.destroy(entity->key());
		}

	private:
		static entt::registry k_Registry;
		static std::unordered_map<uint32_t, Ref<Entity>> k_EntCache;
	};
}

#endif