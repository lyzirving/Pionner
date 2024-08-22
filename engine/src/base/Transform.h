#ifndef __PIONNER_BASE_TRANSFORM_H__
#define __PIONNER_BASE_TRANSFORM_H__

#include "EulerAngle.h"
#include "Position3d.h"

namespace pio
{
	class Transform
	{
	public:
		Transform() {};
		~Transform() = default;

		Transform operator*(const Transform& rhs)
		{
			Transform ret;
			ret.Position = this->Position + rhs.Position;
			ret.Euler = this->Euler * rhs.Euler;
			ret.Scale = this->Scale * rhs.Scale;
			return ret;
		}

		glm::mat4 mat() const { return glm::translate(glm::mat4(1.f), Position.ccs()) * 
								       Euler.mat() *
								       glm::scale(glm::mat4(1.f), Scale); }
	public:
		glm::vec3  Scale{ 1.f };	
		EulerAngle Euler;
		Position3d  Position{};
	};
}

#endif