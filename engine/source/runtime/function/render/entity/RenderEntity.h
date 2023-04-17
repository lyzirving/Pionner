#ifndef __RENDER_ENTITY_H__
#define __RENDER_ENTITY_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "core/math/AABB.h"

#include "function/render/material/Material.h"
#include "function/render/entity/EntityDef.h"

namespace Pionner
{
	class EntityPart
	{
	public:
		EntityPart();
		~EntityPart();

		uint32_t              m_partIndex;
		std::vector<Vertex>   m_vertexs;
		std::vector<uint32_t> m_indices;
		Material              m_material;
		AABB                  m_aabb;
		glm::mat4             m_modelMat;
	};

	class RenderEntity
	{
	public:
		RenderEntity();
		~RenderEntity();

		EntityType m_type;
		DrawOrder  m_order;
		uint32_t   m_entityId;
		uint32_t   m_childNum;
		std::vector<std::shared_ptr<EntityPart>>   m_parts;
		std::vector<std::shared_ptr<RenderEntity>> m_children;
		std::weak_ptr<RenderEntity>                m_parent;

	private:
		static uint32_t g_entityId;
	};
}

#endif