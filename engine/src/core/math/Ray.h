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
		~Ray() = default;

		glm::vec3 at(float t) { return Origin + t * Dir; }

	public:
		/*
		* @brief: Build a ray in world space by click on screen.
		*         The origin of ray is camera's position. The dest point of ray is constructed by touch point, 
		*         and touch point is re-projected back on near plane in camera space.
		*		  The result ray is transformed into world space.
		* @param viewportPt: touch point on screen, which is viewport coordinate, viewport's (0, 0) is left bottom
		* @param camera: camera of current screen
		*/
		static Ray BuildFromScreen(const glm::vec2 &viewportPt, const Camera &camera);

	public:
		glm::vec3 Origin{ 0.f };
		glm::vec3 Dir{ 0.f, 0.f, -1.f };
	};
}

#endif