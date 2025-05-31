#pragma once

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
		inline bool IsZero(const glm::vec4& val) { return IsZero(val.x) && IsZero(val.y) && IsZero(val.z) && IsZero(val.w); }

		glm::vec3 Reminder(const glm::vec3& input, float reminder);
		/*
		* @brief Calculate angle from start direction to end direction.
		*	     Rotation in counter clock-wise is marked as positive.
		* @param start Start direction
		* @param end   End direction
		* @return	   Angle in degree oriented by ccw.
		*/
		float AngleCCW(const glm::vec3& start, const glm::vec3& end);

		/*
		* @brief Calculate camera's pose by camera's view direction, its based on Y-up principle
		* @param viewDir: input param, camera's view direction
		* @param cameraRight: output param, camera's right axis
		* @param cameraUp: output param, camera's up axis
		* @param cameraForward: output param, camera's forward axis
		*/
		void CalcCameraPose(const glm::vec3& viewDir, /*out*/glm::vec3& cameraRight, 
							/*out*/glm::vec3& cameraUp, /*out*/glm::vec3& cameraForward);

		/*
		* @brief Calculate camera's view matrix
		*/
		glm::mat4 CameraViewMat(const glm::vec3& position, const glm::vec3& viewDir, const glm::vec3& cameraUp);
		/*
		* @brief Calculate billboard rotaton from camera's view matrix
		* @param camViewMat: camera's view matrix
		* @return euler angle in degrees.
		*/
		glm::vec3 BillboardRotation(const glm::mat4& camViewMat);
		/*
		* @brief Calculate perspective frustum's matrix
		* @param fov     Fov in angle
		* @param aspect  aspect ratio of the frustum
		* @param near    near edge of frustum
		* @param far     far edge of frustum
		*/
		glm::mat4 PerspectiveMat(float fov, float aspect, float near, float far);
		glm::mat4 OrthogonalMat(float left, float right, float bottom, float top, float near, float far);

		template<typename T>
		T Clamp(T val, T min, T max) { return (val < min) ? min : ((val > max) ? max : val); }

		template<typename T>
		T Length(T lhs, T rhs) { return std::sqrt(lhs * lhs + rhs * rhs); }

		template<typename T>
		bool Equal(const T& lhs, const T& rhs) { return lhs == rhs; }

		template<typename T>
		bool Equal(typename std::remove_reference<T>::type lhs,
				   typename std::remove_reference<T>::type rhs)
		{
			return lhs == rhs;
		}

		template<>
		inline bool Equal(const float& lhs, const float& rhs) { return IsZero(lhs - rhs); }

		template<>
		inline bool Equal(const double& lhs, const double& rhs) { return IsZero(lhs - rhs); }

		inline bool LessEqual(float lhs, float rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool LessEqual(double lhs, double rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool GreatEqual(float lhs, float rhs) { return Equal(lhs, rhs) || lhs > rhs; }
		inline bool GreatEqual(double lhs, double rhs) { return Equal(lhs, rhs) || lhs > rhs; }		
	}

	/*
	* Quaternion is defined by a vec4:
	*	x = RotationAxis.x * sin(RotationAngle / 2)
	*	y = RotationAxis.y * sin(RotationAngle / 2)
	*	z = RotationAxis.z * sin(RotationAngle / 2)
	*	w = cos(RotationAngle / 2)
	* So rotation angle and axis can be calculated.
	* The fllowing code shows how to Calculate RotationAngle(in radians):
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

	inline bool operator<(const glm::vec2& lhs, const glm::vec2& rhs)
	{
		return lhs.x < rhs.x && lhs.y < rhs.y;
	}

	inline bool operator<(const glm::vec3& lhs, const glm::vec3& rhs)
	{
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
	}

	inline bool operator<(const glm::vec4& lhs, const glm::vec4& rhs)
	{
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
	}

	inline bool operator<=(const glm::vec2& lhs, const glm::vec2& rhs)
	{
		return Math::LessEqual(lhs.x, rhs.x) && Math::LessEqual(lhs.y, rhs.y);
	}

	inline bool operator<=(const glm::vec3& lhs, const glm::vec3& rhs)
	{
		return Math::LessEqual(lhs.x, rhs.x) && Math::LessEqual(lhs.y, rhs.y) && Math::LessEqual(lhs.z, rhs.z);
	}

	inline bool operator<=(const glm::vec4& lhs, const glm::vec4& rhs)
	{
		return Math::LessEqual(lhs.x, rhs.x) && Math::LessEqual(lhs.y, rhs.y) && Math::LessEqual(lhs.z, rhs.z) && Math::LessEqual(lhs.w, rhs.w);
	}

	inline bool operator>(const glm::vec2& lhs, const glm::vec2& rhs)
	{
		return lhs.x > rhs.x && lhs.y > rhs.y;
	}

	inline bool operator>(const glm::vec3& lhs, const glm::vec3& rhs)
	{
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
	}

	inline bool operator>(const glm::vec4& lhs, const glm::vec4& rhs)
	{
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
	}

	inline bool operator>=(const glm::vec2& lhs, const glm::vec2& rhs)
	{
		return Math::GreatEqual(lhs.x, rhs.x) && Math::GreatEqual(lhs.y, rhs.y);
	}

	inline bool operator>=(const glm::vec3& lhs, const glm::vec3& rhs)
	{
		return Math::GreatEqual(lhs.x, rhs.x) && Math::GreatEqual(lhs.y, rhs.y) && Math::GreatEqual(lhs.z, rhs.z);
	}

	inline bool operator>=(const glm::vec4& lhs, const glm::vec4& rhs)
	{
		return Math::GreatEqual(lhs.x, rhs.x) && Math::GreatEqual(lhs.y, rhs.y) && Math::GreatEqual(lhs.z, rhs.z) && Math::GreatEqual(lhs.w, rhs.w);
	}
}