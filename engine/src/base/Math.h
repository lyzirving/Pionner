#ifndef __PIONNER_BASE_MATH_LIB_H__
#define __PIONNER_BASE_MATH_LIB_H__

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace pio
{
	#define PIO_PI      (3.141592654f)
	#define PIO_EPSILON (1e-4)

	namespace Math
	{	
		inline glm::vec3 Scale(const glm::vec3 &v, float desiredLength) { return v * desiredLength / glm::length(v); }
		inline bool IsZero(float val)  { return std::fabs(val) < PIO_EPSILON; }
		inline bool IsZero(double val) { return std::fabs(val) < PIO_EPSILON; }
		inline bool IsZero(const glm::vec2 &val) { return IsZero(val.x) && IsZero(val.y); }
		inline bool IsZero(const glm::vec3 &val) { return IsZero(val.x) && IsZero(val.y) && IsZero(val.z); }
		inline bool Equal(float lhs, float rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(double lhs, double rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec2 &lhs, const glm::vec2 &rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec3 &lhs, const glm::vec3 &rhs) { return IsZero(lhs - rhs); }
		inline bool LessEqual(float lhs, float rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool LessEqual(double lhs, double rhs) { return Equal(lhs, rhs) || lhs < rhs; }
		inline bool LessEqual(const glm::vec3 &lhs, const glm::vec3 &rhs) { return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z; }
		inline bool GreatEqual(const glm::vec3 &lhs, const glm::vec3 &rhs) { return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z; }

		template<typename T>
		T Clamp(T val, T min, T max) { return (val < min) ? min : ((val > max) ? max : val); }

		template<typename T>
		T Length(T lhs, T rhs) { return std::sqrt(lhs * lhs + rhs * rhs); }
	}
}

#endif