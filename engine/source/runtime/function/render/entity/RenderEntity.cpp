#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	uint32_t RenderEntity::g_entityId{ 0 };

	EntityPart::EntityPart()
		: m_partIndex(0)
		, m_vertexBufId(0), m_indicesBufId(0)
		, m_vertexs(), m_indices()
		, m_material()
		, m_aabb()
		, m_modelMat(1.f)
	{
	}

	EntityPart::~EntityPart()
	{
		std::vector<Vertex>().swap(m_vertexs);
		std::vector<uint32_t>().swap(m_indices);
	}

	RenderEntity::RenderEntity()
		: m_type(ENTITY_NONE)
		, m_order(ORDER_LOWEST)
		, m_entityId(g_entityId++), m_childNum(0)
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