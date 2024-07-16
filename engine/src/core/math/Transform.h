#ifndef __PIONNER_CORE_MATH_TRANSFORM_H__
#define __PIONNER_CORE_MATH_TRANSFORM_H__

#include "EulerAngle.h"
#include "CPosition.h"

namespace pio
{
	class Transform
	{
	public:
		Transform() {};
		~Transform() = default;

		Transform operator*(const Transform &rhs);

		glm::mat4 mat() const { return glm::translate(glm::mat4(1.f), Position.ccs()) * 
								       Euler.mat() *
								       glm::scale(glm::mat4(1.f), Scale); }
	public:
		glm::vec3  Scale{ 1.f };	
		EulerAngle Euler;
		CPosition  Position{};
	};
}

#endif