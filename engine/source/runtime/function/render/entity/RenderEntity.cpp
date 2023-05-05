#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	uint32_t RenderEntity::g_entityId{ 0 };

	EntityPart::EntityPart()
		: m_partIndex(0)
		, m_vertexSlot(0), m_indicesSlot(0)
		, m_material()
		, m_aabb()
		, m_modelMat(1.f)
	{
	}

	EntityPart::~EntityPart()
	{
		//TODO: release the buffer 
	}

	RenderEntity::RenderEntity()
		: m_type(ENTITY_NONE)
		, m_order(ORDER_LOWEST)
		, m_entityId(g_entityId++)
		, m_parts()
		, m_children()
		, m_parent()
	{
	}

	RenderEntity::~RenderEntity()
	{
		auto itr0 = m_parts.begin();
		while (itr0 != m_parts.end())
		{
			(*itr0).reset();
			itr0 = m_parts.erase(itr0);
		}

		auto itr1 = m_children.begin();
		while (itr1 != m_children.end())
		{
			(*itr1).reset();
			itr1 = m_children.erase(itr1);
		}
		m_parent.reset();
	}
}