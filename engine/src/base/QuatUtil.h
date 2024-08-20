#ifndef __PIONNER_BASE_QUAT_UTIL_H__
#define __PIONNER_BASE_QUAT_UTIL_H__

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Math.h"

/*
 * Quaternion is defined by a vec4:
 *		x = RotationAxis.x * sin(RotationAngle / 2)
 *		y = RotationAxis.y * sin(RotationAngle / 2)
 *		z = RotationAxis.z * sin(RotationAngle / 2)
 *		w = cos(RotationAngle / 2)
 * So rotation angle and axis can be calculated.
 * The fllowing code shows how to calculate RotationAngle(in radians):
 *		RotationAngle = 2 * acos(w)
 * 
 * In GLM, quaternion is organized in vec4(w, x, y, z).
 * 
 * Common use case for quaternion:
 *		// Creates an identity quaternion (no rotation)
 *		glm::quat MyQuaternion;
 * 
 *		// Direct specification of the 4 components
 *		// You almost never use this directly
 *		MyQuaternion = glm::quat(w,x,y,z);
 * 
 *		// Conversion from Euler angles (in radians) to Quaternion
 *		// X(pitch), Y(yaw), Z(roll)
 *		vec3 EulerAngles( RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);
 *		MyQuaternion = glm::quat(EulerAngles);
 * 
 *		// Conversion from axis-angle
 *		// In GLM the angle must be in degrees here, so convert it.
 *		MyQuaternion = gtx::quaternion::angleAxis(glm::degrees(RotationAngle), RotationAxis);
 * 
 *		// Rotate a point
 *		rotated_point = orientation_quaternion *  point;
 * 
 *		// Rotate a point by a specific anchor
 *		rotated_point = origin + (orientation_quaternion * (point-origin));
 * 
 *		// Accumulate two rotation
 *		glm::quat combined_rotation = second_rotation * first_rotation;
 */

namespace pio
{
	namespace quaternion
	{
		constexpr static const glm::quat IDENTITY = glm::quat(1.f, 0.f, 0.f, 0.f);

		bool Equal(const glm::quat &lhs, const glm::quat &rhs);
		glm::quat Mix(const glm::quat &lhs, const glm::quat &rhs, float ratio);
		glm::quat RotationToQuat(const glm::vec3 &start, const glm::vec3 &dest);
	}
}

#endif