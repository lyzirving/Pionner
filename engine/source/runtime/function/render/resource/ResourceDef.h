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
		BUF_TEXTURE,
		BUF_CNT
	};

	enum DataType : uint8_t
	{
		DATA_VERTEX,
		DATA_INDICE,
		DATA_TEXTURE,
		DATA_TYPE_COUNT
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	enum ATTACHMENT : uint8_t
	{
		COLOR_ATTACH_0,
		COLOR_ATTACH_1,
		COLOR_ATTACH_2,
		COLOR_ATTACH_3,
		COLOR_ATTACH_4,
		DEPTH_ATTACH,
		ATTACHMENT_COUNT
	};
}

#endif