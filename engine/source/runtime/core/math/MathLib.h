#ifndef __CORE_MATH_LIB_H__
#define __CORE_MATH_LIB_H__

#include <glm/glm.hpp>

namespace Pionner
{
	#define MATH_PI 3.14159265358979323846

	class MathLib
	{
	public:
		// spherical coordinate system to cartesian coordinate system
		static void SCStoCCS(float theta, float phi, float r, glm::vec3 &out);
		// cartesian coordinate system to spherical coordinate system
		static void CCStoSCS(const glm::vec3 &ccs, glm::vec3 &out);

		static bool nearZeroF(float val);
		static bool nearZeroVec3(const glm::vec3 &val);

		static bool equalF(float lhs, float rhs);
		static bool equalVec3(const glm::vec3 &lhs, const glm::vec3 &rhs);

		static glm::mat4 normalMat(const glm::mat4 &modelMat);

	private:
		MathLib() {}
		~MathLib() = default;
	};
}

#endif