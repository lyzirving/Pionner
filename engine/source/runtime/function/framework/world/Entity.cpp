#include "function/framework/world/Entity.h"

namespace Pionner
{
	Entity::Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id)
		: m_name("entity_" + std::to_string(id)), m_id(), m_worldImpl(world)
	{
	}

	Entity::Entity(std::shared_ptr<decs::ECSWorld> &world, uint32_t id, const std::string &name)
		: m_name("entity_" + std::to_string(id) + "_" + name), m_id(), m_worldImpl(world)
	{
	}

	Entity::~Entity()
	{
		m_worldImpl.reset();
	}
}