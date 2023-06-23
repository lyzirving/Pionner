#include <glm/gtx/transform.hpp>

#include "RenderEntity.h"
#include "ModelEntity.h"
#include "LightEntity.h"

#include "function/global/GlobalContext.h"

#include "function/framework/load/Loader.h"

#include "function/render/RenderSystem.h"

#include "function/render/rhi/Rhi.h"


namespace Pionner
{
	uint32_t RenderEntity::g_entityId{ 0 };

	EntityPart::EntityPart()
		: m_partIndex(0)
		, m_vertexSlot(-1), m_indicesSlot(-1)
		, m_material()
		, m_aabb()
		, m_transComp()
		, m_owner(nullptr)
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
			g_runtimeCtx.m_renderSystem->notifyRelease(BUF_VERTEX, m_vertexSlot);
		}
		if (indiceSlotValid())
		{
			g_runtimeCtx.m_renderSystem->notifyRelease(BUF_INDICE, m_indicesSlot);
		}
		if (m_material.slotValid())
		{
			g_runtimeCtx.m_renderSystem->notifyRelease(BUF_TEXTURE, m_material.m_slot);
		}
		m_owner.reset();
	}

	glm::vec3 EntityPart::center()
	{
		glm::vec3 aa = m_aabb.AA();
		glm::vec3 bb = m_aabb.BB();
		glm::vec3 ret = (aa + bb) * 0.5f;
		return ret;
	}

	glm::mat4 EntityPart::getTransform()
	{
		glm::mat4 matrix = m_transComp.getMat();
		auto parent = m_owner;
		while (parent)
		{
			matrix = parent->m_transComp.getMat() * matrix;
			parent = parent->m_parent;
		}
		return matrix;
	}

	RenderEntity::RenderEntity()
		: m_name()
		, m_type(RENDER_ENTITY_TYPE_MODEL)
		, m_order(ORDER_LOWEST)
		, m_entityId(g_entityId++)
		, m_transComp()
		, m_parent(nullptr)
		, m_parts()
		, m_children()
	{
	}

	RenderEntity::~RenderEntity()
	{
		auto itr0 = m_children.begin();
		while (itr0 != m_children.end())
		{
			(*itr0).reset();
			itr0 = m_children.erase(itr0);
		}

		auto itr1 = m_parts.begin();
		while (itr1 != m_parts.end())
		{
			(*itr1).reset();
			itr1 = m_parts.erase(itr1);
		}
		m_parent.reset();
	}

	std::shared_ptr<RenderEntity> RenderEntity::makeEntity(RenderEntityType type)
	{
		std::shared_ptr<RenderEntity> result{ nullptr };
		switch (type)
		{
			case Pionner::RENDER_ENTITY_TYPE_MODEL:
				result = std::shared_ptr<RenderEntity>(new ModelEntity);
				break;
			case Pionner::RENDER_ENTITY_TYPE_LIGHT:
				result = std::shared_ptr<RenderEntity>(new LightEntity);
				break;
			default:
				break;
		}
		return result;
	}

	void RenderEntity::draw(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto cmd = rhi->getDrawCmd();

		Blend blend = Blend::disable();
		DepthTest depth = DepthTest::common();
		CullFace cullFace = CullFace::common();

		rhi->setCullMode(cullFace);
		rhi->setDepthMode(depth);
		rhi->setBlendMode(blend);

		cmd->drawEntity(*this, param);
	}
}