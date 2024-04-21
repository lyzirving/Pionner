#ifndef __PIONNER_CORE_MATH_AABB_H__
#define __PIONNER_CORE_MATH_AABB_H__

#include "core/func/Drawable.h"

namespace pio
{
	class LineMesh;

	/*
	 * Point layout of a AABB
	 *       v7 ----------- v6
	 *      /|            / |
	 *     / |           /  |
	 *   v4 -|--------- v5  |
	 *    |  v3 --------|-- v2
	 *    | /           |  /
	 *    |/            | /
	 *	  /             |/
	 *    v0 ---------- v1
	 */
	class AABB : public Drawable
	{
	public:
		AABB();
		AABB(const glm::vec3 &min, const glm::vec3 &max);

		virtual void onCreateMesh() override;
		virtual void onDraw(const DrawParam &param) override;

		glm::vec3 center() const { return (Min + Max) * 0.5f; }

		//[NOTE]: It's weird that some model's max is smaller than min after model transformation.
		//        So i use std::abs() to make sure the result is positive
		float lenX() const { return std::abs(Max.x - Min.x); }
		float lenY() const { return std::abs(Max.y - Min.y); }
		float lenZ() const { return std::abs(Max.z - Min.z); }

		void updatePoints()
		{
			glm::vec3 len = glm::vec3(lenX(), lenY(), lenZ());
			m_points[3] = Min;
			m_points[2] = m_points[3] + AXIS_X * len.x;
			m_points[0] = m_points[3] + AXIS_Z * len.z;
			m_points[1] = m_points[0] + AXIS_X * len.x;

			m_points[5] = Max;
			m_points[4] = m_points[5] - AXIS_X * len.x;
			m_points[6] = m_points[5] - AXIS_Z * len.z;
			m_points[7] = m_points[4] - AXIS_Z * len.z;
		}

		void reset() { Min = glm::vec3(FLT_MAX); Max = glm::vec3(-FLT_MAX); }

	public:
		glm::vec3 Min{ FLT_MAX }, Max{ -FLT_MAX };

	private:
		glm::vec3     m_points[8]{ glm::vec3(0.f) };
		Ref<LineMesh> m_outline;
	};
}

#endif