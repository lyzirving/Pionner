#ifndef __ENTITY_DEF_H__
#define __ENTITY_DEF_H__

#include <glm/glm.hpp>

namespace Pionner
{
	enum EntityType : uint8_t
	{
		ENTITY_NONE,
		ENTITY_MODEL,
		ENTITY_TYPE_COUNT
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};
}

#endif