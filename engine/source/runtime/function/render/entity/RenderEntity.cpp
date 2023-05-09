#include "function/render/entity/RenderEntity.h"
#include "function/global/GlobalContext.h"
#include "function/render/RenderSystem.h"

namespace Pionner
{
	uint32_t RenderEntity::g_entityId{ 0 };

	EntityPart::EntityPart()
		: m_partIndex(0)
		, m_vertexSlot(-1), m_indicesSlot(-1)
		, m_material()
		, m_aabb()
		, m_modelMat(1.f)
	{
	}

	EntityPart::~EntityPart()
	{
		if (!g_runtimeCtx.m_renderSystem)
		{
			return;
		}

		if (vetexSlotValid())
		{
			g_runtimeCtx.m_renderSystem->deleteResource(DATA_VERTEX, m_vertexSlot);
		}
		if (indiceSlotValid())
		{
			g_runtimeCtx.m_renderSystem->deleteResource(DATA_INDICE, m_indicesSlot);
		}
		if (m_material.slotValid())
		{
			g_runtimeCtx.m_renderSystem->deleteResource(DATA_TEXTURE, m_material.m_slot);
		}
	}

	glm::vec3 EntityPart::center()
	{
		glm::vec3 aa = m_aabb.AA();
		glm::vec3 bb = m_aabb.BB();
		glm::vec3 ret = (aa + bb) * 0.5f;
		return ret;
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