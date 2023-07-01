#ifndef __ENTITY_DEF_H__
#define __ENTITY_DEF_H__

#include <glm/glm.hpp>

namespace Pionner
{
	enum RenderMode : uint8_t
	{
		RENDER_MODE_WIRE_FRAME,
		RENDER_MODE_SOLID,
		RENDER_MODE_MATERIAL_DISPLAY,
		RENDER_MODE_RENDERED_DISPLAY,
		RENDER_MODE_CNT
	};

	enum RenderEntityType : uint8_t
	{
		RENDER_ENTITY_TYPE_MODEL,
		RENDER_ENTITY_TYPE_COUNT
	};

	enum DrawOrder : uint8_t
	{
		ORDER_LOWEST,
		ORDER_LOW_1,
		ORDER_LOW_2,
		ORDER_MIDDLE_3,
		ORDER_MIDDLE_4,
		ORDER_HIGH_5,
		ORDER_HIGH_6,
		ORDER_TOP,
		ORDER_COUNT
	};
}

#endif