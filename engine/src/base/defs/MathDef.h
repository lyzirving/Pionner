#ifndef __PIONNER_BASE_DEFS_MATH_DEF_H__
#define __PIONNER_BASE_DEFS_MATH_DEF_H__

#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pio
{
	#define PIO_PI      (3.141592654f)
	#define PIO_EPSILON (1e-4)

	namespace Math
	{
		inline glm::vec3 Scale(const glm::vec3& v, float desiredLength) { return v * desiredLength / glm::length(v); }
		inline bool IsZero(float val) { return std::fabs(val) < PIO_EPSILON; }
		inline bool IsZero(double val) { return std::fabs(val) < PIO_EPSILON; }
		inline bool IsZero(const glm::vec2& val) { return IsZero(val.x) && IsZero(val.y); }
		inline bool IsZero(const glm::vec3& val) { return IsZero(val.x) && IsZero(val.y) && IsZero(val.z); }
		inline bool Equal(float lhs, float rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(double lhs, double rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec2& lhs, const glm::vec2& rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec3& lhs, const glm::vec3& rhs) { return IsZero(lhs - rhs); }
		inline bool LessEqual(float lhs, float rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool LessEqual(double lhs, double rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool LessEqual(const glm::vec3& lhs, const glm::vec3& rhs) { return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z; }
		inline bool GreatEqual(const glm::vec3& lhs, const glm::vec3& rhs) { return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z; }		

		template<typename T>
		T Clamp(T val, T min, T max) { return (val < min) ? min : ((val > max) ? max : val); }

		template<typename T>
		T Length(T lhs, T rhs) { return std::sqrt(lhs * lhs + rhs * rhs); }

		glm::vec3 Reminder(const glm::vec3& input, float reminder);
		/*
		* @brief		Calculate angle from start direction to end direction.
		*				Rotation in counter clock-wise is marked as positive.
		* @param start	Start direction
		* @param end	End direction
		* @return		Angle in degree oriented by ccw.
		*/
		float AngleCCW(const glm::vec3& start, const glm::vec3& end);
	}

	/*
	* Quaternion is defined by a vec4:
	*	x = RotationAxis.x * sin(RotationAngle / 2)
	*	y = RotationAxis.y * sin(RotationAngle / 2)
	*	z = RotationAxis.z * sin(RotationAngle / 2)
	*	w = cos(RotationAngle / 2)
	* So rotation angle and axis can be calculated.
	* The fllowing code shows how to calculate RotationAngle(in radians):
	*	RotationAngle = 2 * acos(w)
	*
	* In GLM, quaternion is organized in vec4(w, x, y, z).
	*
	* Common use case for quaternion:
	*	// Creates an identity quaternion (no rotation)
	*	glm::quat MyQuaternion;
	*
	*	// Direct specification of the 4 components
	*	// You almost never use this directly
	*	MyQuaternion = glm::quat(w,x,y,z);
	*
	*	// Conversion from Euler angles (in radians) to Quaternion
	*	// X(pitch), Y(yaw), Z(roll)
	*	vec3 EulerAngles( RotationAroundXInRadians, RotationAroundYInRadians, RotationAroundZInRadians);
	*	MyQuaternion = glm::quat(EulerAngles);
	*
	*	// Conversion from axis-angle
	*	// In GLM the angle must be in degrees here, so convert it.
	*	MyQuaternion = gtx::quaternion::angleAxis(glm::degrees(RotationAngle), RotationAxis);
	*
	*	// Rotate a point
	*	rotated_point = orientation_quaternion *  point;
	*
	*	// Rotate a point by a specific anchor
	*	rotated_point = origin + (orientation_quaternion * (point-origin));
	*
	*	// Accumulate two rotation
	*	glm::quat combined_rotation = second_rotation * first_rotation;
	*/
	namespace quaternion
	{
		constexpr static const glm::quat IDENTITY = glm::quat(1.f, 0.f, 0.f, 0.f);

		bool Equal(const glm::quat& lhs, const glm::quat& rhs);
		glm::quat Mix(const glm::quat& lhs, const glm::quat& rhs, float ratio);
		glm::quat RotationToQuat(const glm::vec3& start, const glm::vec3& dest);
	}
}

#endif