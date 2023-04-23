#ifndef __RESOURCE_DEF_H__
#define __RESOURCE_DEF_H__

#include <cstdint>
#include <glm/glm.hpp>

namespace Pionner
{
	enum BufferType : uint8_t
	{
		BUF_MEM_ARRAY,
		BUF_VBO,
		BUF_EBO,
		BUF_CNT
	};

	struct GfxVertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};
}

#endif