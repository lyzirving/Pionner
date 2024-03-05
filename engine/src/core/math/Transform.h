#ifndef __PIONNER_CORE_MATH_TRANSFORM_H__
#define __PIONNER_CORE_MATH_TRANSFORM_H__

#include "core/Base.h"
#include "core/math/EulerAngle.h"

namespace pio
{
	class Transform
	{
	public:
		Transform() {};
		~Transform() = default;

		Transform operator*(const Transform &rhs);

		glm::mat4 mat() const { return glm::translate(glm::mat4(1.f),  Position) * 
								       Euler.mat() *
								       glm::scale(glm::mat4(1.f), Scale); }
	public:
		glm::vec3 Position{ 0.f };
		glm::vec3 Scale{ 1.f };	
		EulerAngle Euler;
	};
}

#endif