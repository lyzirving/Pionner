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

		//[NOTE]: It's weird that some model's max is smaller than min after model transformation.
		//        So i use std::abs() to make sure the result is positive
		float lenX() const { return std::abs(Max.x - Min.x); }
		float lenY() const { return std::abs(Max.y - Min.y); }
		float lenZ() const { return std::abs(Max.z - Min.z); }

		static AABB Multiply(const glm::mat4 &mat, const AABB &aabb)
		{
			return AABB(mat * glm::vec4(aabb.Min, 1.f), mat * glm::vec4(aabb.Max, 1.f));
		}
	};
}

#endif