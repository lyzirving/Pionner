#ifndef __RENDER_ENTITY_H__
#define __RENDER_ENTITY_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "core/math/AABB.h"

#include "function/framework/comp/TransformComp.h"

#include "function/render/material/Material.h"
#include "function/render/entity/EntityDef.h"

namespace Pionner
{
	class  GfxBuffer;
	class  Shader;
	class  RenderEntity;
	struct RenderParam;

	class EntityPart
	{
	public:
		EntityPart();
		~EntityPart();

		glm::vec3 center();

		bool vetexSlotValid() const { return m_vertexSlot >= 0; }
		bool indiceSlotValid() const { return m_indicesSlot >= 0; }
		glm::mat4 getTransform();

	public:
		uint32_t                      m_partIndex;
		int32_t                       m_vertexSlot;
		int32_t                       m_indicesSlot;
		Material                      m_material;
		AABB                          m_aabb;
		TransformComp                 m_transComp;
		std::shared_ptr<RenderEntity> m_owner;
	};

	class RenderEntity
	{
	public:
		RenderEntity();
		virtual ~RenderEntity();

		static std::shared_ptr<RenderEntity> makeEntity(RenderEntityType type);

		virtual void draw(RenderParam &param);

		virtual bool dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								/*out*/std::shared_ptr<Shader> &shader, /*out*/std::shared_ptr<GfxBuffer> &texture) = 0;
		virtual bool dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
									 /*out*/std::shared_ptr<Shader> &shader) = 0;

	public:
		std::string      m_name;
		RenderEntityType m_type;
		DrawOrder        m_order;
		uint32_t         m_entityId;
		TransformComp    m_transComp;
		std::shared_ptr<RenderEntity>              m_parent;
		std::vector<std::shared_ptr<EntityPart>>   m_parts;
		std::vector<std::shared_ptr<RenderEntity>> m_children;

	private:
		static uint32_t g_entityId;
	};
}

#endif