#ifndef __ENTITY_DEF_H__
#define __ENTITY_DEF_H__

#include <glm/glm.hpp>

namespace Pionner
{
	enum EntityType : uint8_t
	{
		ENTITY_TYPE_NONE,
		ENTITY_TYPE_MODEL,
		ENTITY_TYPE_LIGHT,
		ENTITY_TYPE_COUNT
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