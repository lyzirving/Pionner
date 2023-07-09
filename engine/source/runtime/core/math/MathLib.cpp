#include <cmath>

#include "core/math/MathLib.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MathLib"

namespace pio
{
	void MathLib::SCStoCCS(float theta, float phi, float r, glm::vec3 &out)
	{
		float projection = std::abs(r * std::sin(glm::radians(theta)));
		out.x = projection * std::sin(glm::radians(phi));
		out.y = r * std::cos(glm::radians(theta));
		out.z = projection * std::cos(glm::radians(phi));
	}

	void MathLib::CCStoSCS(const glm::vec3 &ccs, glm::vec3 &out)
	{
		float len = std::sqrt(std::pow(ccs.x, 2) + std::pow(ccs.y, 2) + std::pow(ccs.z, 2));
		out.x = glm::degrees(std::acos(ccs.y / len));
		out.z = len;

		// tan(a+b) = (tan(a) + tan(b)) / (1 - tan(a)tan(b))
		// tan(a-b) = (tan(a) - tan(b)) / (1 + tan(a)tan(b))
		if (ccs.z == 0.f)
		{
			if (ccs.x == 0.f)
			{
				LOG_ERR("err input for ccs, x and z can not be 0.f at the same time");
				return;
			}

			if (ccs.x > 0.f)
				out.y = 90.f;
			else
				out.y = 270.f;
		}
		else
		{
			float tanTheta{ 0.f };
			if (ccs.x >= 0.f && ccs.z > 0.f)
			{
				tanTheta = ccs.x / ccs.z;
				out.y = glm::degrees(std::atan(tanTheta));
			}
			else if (ccs.x >= 0.f && ccs.z < 0.f)
			{
				tanTheta = ccs.x / std::abs(ccs.z);
				out.y = 180.f - glm::degrees(std::atan(tanTheta));
			}
			else if (ccs.x <= 0.f && ccs.z < 0.f)
			{
				tanTheta = ccs.x / ccs.z;
				out.y = 180.f + glm::degrees(std::atan(tanTheta));
			}
			else if (ccs.x <= 0.f && ccs.z > 0.f)
			{
				tanTheta = std::abs(ccs.x) / ccs.z;
				out.y = 360.f - glm::degrees(std::atan(tanTheta));
			}
		}
	}

	bool MathLib::equalZeroF(float val)
	{
		return std::abs(val) < 1e-4;
	}

	bool MathLib::equalZeroVec3(const glm::vec3 &val)
	{
		return equalZeroF(val.x) && equalZeroF(val.y) && equalZeroF(val.z);
	}

	bool MathLib::equalF(float lhs, float rhs)
	{
		return equalZeroF(lhs - rhs);
	}

	bool MathLib::equalVec3(const glm::vec3 &lhs, const glm::vec3 &rhs)
	{
		return equalF(lhs.x, rhs.x) && equalF(lhs.y, rhs.y) && equalF(lhs.z, rhs.z);
	}

	glm::mat3 MathLib::normalMat(const glm::mat4 &modelMat)
	{
		glm::mat3 ret = glm::mat3(glm::transpose(glm::inverse(modelMat)));
		return ret;
	}
}