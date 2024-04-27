#ifndef __PIONNER_CORE_MATH_MATH_LIB_H__
#define __PIONNER_CORE_MATH_MATH_LIB_H__

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace pio
{
	#define PIO_PI (3.141592654f)

	struct Viewport;
	struct Rect2d;

	namespace Math
	{
		bool Contains(const glm::vec2 &cursor, const Rect2d &rect);
		bool DecomposeTransform(const glm::mat4 &transform, glm::vec3 &translation, glm::quat &rotation, glm::vec3 &scale);	
		glm::vec3 Reminder(const glm::vec3 &input, float reminder);

		inline glm::vec3 Scale(const glm::vec3 &v, float desiredLength) { return v * desiredLength / glm::length(v); }

		inline bool IsZero(float val)  { return std::fabs(val) < 1e-4; }
		inline bool IsZero(double val) { return std::fabs(val) < 1e-4; }
		inline bool IsZero(const glm::vec2 &val) { return IsZero(val.x) && IsZero(val.y); }
		inline bool IsZero(const glm::vec3 &val) { return IsZero(val.x) && IsZero(val.y) && IsZero(val.z); }

		inline bool Equal(float lhs, float rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec2 &lhs, const glm::vec2 &rhs) { return IsZero(lhs - rhs); }
		inline bool Equal(const glm::vec3 &lhs, const glm::vec3 &rhs) { return IsZero(lhs - rhs); }
		inline bool LessEqual(const glm::vec3 &lhs, const glm::vec3 &rhs) { return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z; }
		inline bool GreatEqual(const glm::vec3 &lhs, const glm::vec3 &rhs) { return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z; }

		inline glm::mat3 NormalMat(const glm::mat4 &modelMat) { return glm::transpose(glm::inverse(glm::mat3(modelMat))); }		

		// greatest common divisor 
		inline int32_t GCB(int32_t a, int32_t b)
		{
			int c = 0;
			while (c = a % b)
			{
				a = b;
				b = c;
			}
			return b;
		}
		// least common multipler
		inline int32_t LCM(int32_t a, int32_t b) { return a * b / GCB(a, b); }

		template<typename T>
		T Clamp(T val, T min, T max) { return (val < min) ? min : ((val > max) ? max : val); }

		template<typename T>
		T Length(T lhs, T rhs) { return std::sqrt(lhs * lhs + rhs * rhs); }
	}
}

#endif