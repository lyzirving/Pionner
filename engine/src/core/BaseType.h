#ifndef __PIONNER_CORE_BASE_TYPE_H__
#define __PIONNER_CORE_BASE_TYPE_H__

#include "core/math/MathLib.h"

namespace pio
{
	struct Rect2d
	{
		glm::vec2 LeftTop{};
		glm::vec2 LeftBottom{};
		glm::vec2 RightTop{};
		glm::vec2 RightBottom{};
	};
}

#endif