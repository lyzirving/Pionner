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

		inline glm::vec3 Reminder(const glm::vec3& input, float reminder)
		{
			if (reminder < 0.f) { reminder = -reminder; }

			glm::vec3 result = input;
			glm::vec3 sign = glm::sign(result);

			auto calcReminder = [](float input, float sign, float reminder)
			{
				float val = input * sign;// val is >= 0.f
				if (val > reminder)
					val = val - reminder * int(val / reminder);
				return val * sign;
			};

			result.x = calcReminder(result.x, sign.x, reminder);
			result.y = calcReminder(result.y, sign.y, reminder);
			result.z = calcReminder(result.z, sign.z, reminder);

			return result;
		}

		template<typename T>
		T Clamp(T val, T min, T max) { return (val < min) ? min : ((val > max) ? max : val); }

		template<typename T>
		T Length(T lhs, T rhs) { return std::sqrt(lhs * lhs + rhs * rhs); }
	}
}

#endif