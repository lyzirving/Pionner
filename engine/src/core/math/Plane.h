#ifndef __PIONNER_CORE_MATH_PLANE_H__
#define __PIONNER_CORE_MATH_PLANE_H__

#include "core/Base.h"

namespace pio
{
	/*
	* @brief: Function of plane: Ax + By + Cz + D = 0, N(A, B, C)
	*         if p0(x0, y0, z0) is on the plane, so D = -(N dot p0)
	*/
	class Plane
	{
	public:
		Plane() {}
		Plane(const glm::vec3 &normal, const glm::vec3 &p0);
		// [NOTE] p0, p1, p2 should be organized in counter-clockwise order
		Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2);
		~Plane() = default;

		bool bOverPlane(const glm::vec3 &p) { return glm::dot(m_N, p) + m_D > 0.f; }
		bool bOnPlane(const glm::vec3 &p) { return Math::Equal(glm::dot(m_N, p) + m_D, 0.f); }

		const glm::vec3 &getNormal() { return m_N; }
		const glm::vec3 &getNormal() const { return m_N; }

	private:
		glm::vec3 m_N{0.f};
		float m_D{ 0.f };
	};
}

#endif