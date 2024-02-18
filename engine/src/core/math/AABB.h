#ifndef __PIONNER_CORE_MATH_AABB_H__
#define __PIONNER_CORE_MATH_AABB_H__

#include "core/Base.h"

namespace pio
{
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB() : Min(0.0f), Max(0.0f) {}

		AABB(const glm::vec3 &min, const glm::vec3 &max) : Min(min), Max(max) {}

		glm::vec3 center() const { return glm::vec3((Min + Max) * 0.5f); }

		float lenX() const { return Max.x - Min.x; }
		float lenY() const { return Max.y - Min.y; }
		float lenZ() const { return Max.z - Min.z; }
	};
}

#endif