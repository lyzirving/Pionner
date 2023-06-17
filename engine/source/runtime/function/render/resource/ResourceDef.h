#ifndef __RESOURCE_DEF_H__
#define __RESOURCE_DEF_H__

#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

namespace Pionner
{
	class GfxBuffer;
	class GfxFrameBuffer;

	enum BufferType : uint8_t
	{
		BUF_VERTEX,
		BUF_INDICE,
		BUF_TEXTURE,
		BUF_DEPTH_FRAMEBUFFER,
		BUF_COLOR_FRAMEBUFFER,
		BUF_CNT
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

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};
}

#endif