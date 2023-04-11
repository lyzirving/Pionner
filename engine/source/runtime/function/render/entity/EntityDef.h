#ifndef __ENTITY_DEF_H__
#define __ENTITY_DEF_H__

#include <glm/glm.hpp>

namespace Pionner
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};
}

#endif