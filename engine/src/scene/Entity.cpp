#include "Entity.h"

namespace pio
{
	uint32_t Entity::k_EntNum = 0;

	#define PIO_MAKE_ENT_NAME (std::string("Entity") + std::to_string(k_EntNum++))

	Entity::Entity(const entt::entity& key, const std::string& name) : m_key(key), m_id()
	{
		if (m_name.empty())
			m_name = PIO_MAKE_ENT_NAME;
		else
			k_EntNum++;
	}
}