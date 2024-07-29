#ifndef __PIONNER_CORE_MATH_RAY_H__
#define __PIONNER_CORE_MATH_RAY_H__

#include "core/Base.h"

namespace pio
{
	class Camera;

	class Ray
	{
	public:
		Ray() {}
		Ray(const glm::vec3 &origin, const glm::vec3 &dir) : Origin(origin), Dir(dir) {}
		Ray(const glm::vec3 &origin, const glm::vec3 &dir, const glm::vec3 &ptOnNearPlane) : Origin(origin), Dir(dir), PtOnNear(ptOnNearPlane) {}
		~Ray() = default;

		glm::vec3 at(float t) { return Origin + t * Dir; }

	public:
		glm::vec3 Origin{ 0.f };
		glm::vec3 Dir{ 0.f, 0.f, -1.f };
		glm::vec3 PtOnNear{ 0.f };
	};
}

#endif