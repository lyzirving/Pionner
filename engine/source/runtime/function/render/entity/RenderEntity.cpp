#include "function/render/entity/RenderEntity.h"

namespace Pionner
{
	uint32_t RenderEntity::g_entityId{ 0 };

	EntityPart::EntityPart() : m_partIndex(0)
		                     , m_meshId(0), m_materialId(0)
		                     , m_modelMat(1.f)
	{
	}

	EntityPart::~EntityPart()
	{
	}

	RenderEntity::RenderEntity() : m_entityId(g_entityId++), m_parts()
	{
	}

	RenderEntity::~RenderEntity()
	{
		auto itr = m_parts.begin();
		while (itr != m_parts.end())
		{
			(*itr).reset();
			itr = m_parts.erase(itr);
		}
	}
}