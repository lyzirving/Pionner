#ifndef __PIONNER_CORE_MATH_OBB_H__
#define __PIONNER_CORE_MATH_OBB_H__

#include "core/Base.h"
#include "core/math/Transform.h"

namespace pio
{
	/*
	 * Vertex of an obb
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
	struct OBB
	{
		glm::vec3 Center{ 0.f };
		glm::vec3 AxisX{ 1.f, 0.f, 0.f };
		glm::vec3 AxisY{ 0.f, 1.f, 0.f };
		glm::vec3 AxisZ{ 0.f, 0.f, 1.f };
		glm::vec3 Len{ 1.f };
		glm::vec3 Points[8];

		Transform Trans;
		bool Dirty{ true };

		const glm::vec3 &at(uint32_t ind) { return Points[Math::Clamp(ind, 0u, 7u)]; }

		void setCenter(const glm::vec3 &val) { Center = val; invalidate(); }
		void setLocalRotation(const EulerAngle &angle)
		{
			glm::mat3 mat = glm::mat3(angle.mat());
			AxisX = mat * AxisX;
			AxisY = mat * AxisY;
			AxisZ = mat * AxisZ;
			invalidate();
		}
		void setLocalRotation(const glm::vec3 &angle) { setLocalRotation(EulerAngle(angle)); invalidate(); }
		void setLength(const glm::vec3 &val) { Len = val; invalidate(); }

		void setTranslation(const glm::vec3 &val) { Trans.Position = val; invalidate(); }
		void setRotation(const EulerAngle &angle) { Trans.Euler = angle; invalidate(); }
		void setRotation(const glm::vec3 &angle) { setRotation(EulerAngle(angle)); invalidate(); }
		void setScale(const glm::vec3 &scale) { Trans.Scale *= scale; invalidate(); }

		void invalidate(bool val = true) { Dirty = val; }

		void updatePoints()
		{
			if (!Dirty)
				return;

			Dirty = false;
			glm::mat3 rotMat = glm::mat3(Trans.Euler.mat());
			glm::vec3 position = Center + Trans.Position;
			glm::vec3 scale = Trans.Scale;
			glm::vec3 x_axis = rotMat * AxisX;
			glm::vec3 y_axis = rotMat * AxisY;
			glm::vec3 z_axis = rotMat * AxisZ;

			Points[0] = position - 0.5f * Len.x * scale.x * x_axis - 0.5f * Len.y * scale.y * y_axis + 0.5f * Len.z * scale.z * z_axis;
			Points[1] = position + 0.5f * Len.x * scale.x * x_axis - 0.5f * Len.y * scale.y * y_axis + 0.5f * Len.z * scale.z * z_axis;
			Points[2] = position + 0.5f * Len.x * scale.x * x_axis - 0.5f * Len.y * scale.y * y_axis - 0.5f * Len.z * scale.z * z_axis;
			Points[3] = position - 0.5f * Len.x * scale.x * x_axis - 0.5f * Len.y * scale.y * y_axis - 0.5f * Len.z * scale.z * z_axis;

			Points[4] = position - 0.5f * Len.x * scale.x * x_axis + 0.5f * Len.y * scale.y * y_axis + 0.5f * Len.z * scale.z * z_axis;
			Points[5] = position + 0.5f * Len.x * scale.x * x_axis + 0.5f * Len.y * scale.y * y_axis + 0.5f * Len.z * scale.z * z_axis;
			Points[6] = position + 0.5f * Len.x * scale.x * x_axis + 0.5f * Len.y * scale.y * y_axis - 0.5f * Len.z * scale.z * z_axis;
			Points[7] = position - 0.5f * Len.x * scale.x * x_axis + 0.5f * Len.y * scale.y * y_axis - 0.5f * Len.z * scale.z * z_axis;
		}
	};
}

#endif