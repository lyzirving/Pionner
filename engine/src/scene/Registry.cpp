#include "Registry.h"

namespace pio
{
	entt::registry Registry::k_Registry{};
	std::unordered_map<uint32_t, Ref<Entity>> Registry::k_EntCache{};

	void Registry::Init() {}

	void Registry::Shutdown()
	{
		auto it = k_EntCache.begin();
		while (it != k_EntCache.end())
		{
			k_Registry.destroy(it->second->key());
			it = k_EntCache.erase(it);
		}
	}
}