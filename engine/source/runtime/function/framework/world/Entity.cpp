#include "Entity.h"

namespace Pionner
{
	Entity::Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id)
		: m_name("entity_" + std::to_string(id)), m_id(), m_worldImpl(world)
		, m_type(ENTITY_OBJ)
	{
	}

	Entity::Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id, const std::string &name)
		: m_name(name), m_id(), m_worldImpl(world), m_type(ENTITY_OBJ)
	{
	}

	Entity::~Entity()
	{
		m_worldImpl.reset();
	}
}