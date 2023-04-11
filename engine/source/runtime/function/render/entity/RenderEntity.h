#ifndef __RENDER_ENTITY_H__
#define __RENDER_ENTITY_H__

#include <vector>
#include <memory>

#include <glm/glm.hpp>

namespace Pionner
{
	class EntityPart
	{
	public:
		EntityPart();
		~EntityPart();

		uint32_t  m_partIndex;
		uint32_t  m_meshId;
		uint32_t  m_materialId;
		glm::mat4 m_modelMat;
	};

	class RenderEntity
	{
	public:
		RenderEntity();
		~RenderEntity();

		uint32_t  m_entityId;
		std::vector<std::shared_ptr<EntityPart>> m_parts;

	private:
		static uint32_t g_entityId;
	};
}

#endif