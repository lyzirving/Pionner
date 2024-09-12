#include "Entity.h"

namespace pio
{
	uint32_t Entity::k_EntNum = 0;

	#define PIO_MAKE_ENT_NAME (std::string("Entity") + std::to_string(k_EntNum++))

	Entity::Entity(const entt::entity& key, entt::registry& regi, EntityType type, const std::string& name)
		: m_key(key), m_regi(regi), m_id(), m_type(type)
	{
		if (name.empty())
			m_name = PIO_MAKE_ENT_NAME;
		else
		{
			m_name = name;
			k_EntNum++;
		}
	}
}