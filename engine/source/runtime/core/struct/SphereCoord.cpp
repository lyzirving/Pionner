#include <cmath>

#include "SphereCoord.h"
#include "core/math/MathLib.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "SphereCoord"

namespace pio
{
	SphereCoord::SphereCoord()
	{
	}

	SphereCoord::SphereCoord(float theta, float phi, float radius)
		: m_theta(theta), m_phi(phi), m_radius(radius)
	{
	}

	SphereCoord::~SphereCoord() = default;

	void SphereCoord::applyCartesian(const glm::vec3 &ccs)
	{
		if (MathLib::equalZeroF(ccs.z) && MathLib::equalZeroF(ccs.x) && MathLib::equalZeroF(ccs.y))
		{
			m_theta = 0.f;
			m_phi = 0.f;
			m_radius = 0.f;
			return;
		}
		else if (MathLib::equalZeroF(ccs.z) && MathLib::equalZeroF(ccs.x))
		{
			m_theta = ccs.y > 0.f ? 0.f : 180.f;
			m_phi = 0.f;
			m_radius = std::abs(ccs.y);
			return;
		}
		else if (MathLib::equalZeroF(ccs.z) && MathLib::equalZeroF(ccs.y))
		{
			m_theta = 90.f;
			m_phi = ccs.x > 0.f ? 90.f : 270.f;
			m_radius = std::abs(ccs.x);
			return;
		}
		else if (MathLib::equalZeroF(ccs.x) && MathLib::equalZeroF(ccs.y))
		{
			m_theta = 90.f;
			m_phi = ccs.z > 0.f ? 0.f : 180.f;
			m_radius = std::abs(ccs.z);
			return;
		}

		float len = std::sqrt(std::pow(ccs.x, 2) + std::pow(ccs.y, 2) + std::pow(ccs.z, 2));

		m_radius = len;
		m_theta  = glm::degrees(std::acos(ccs.y / len));

		if (MathLib::equalZeroF(ccs.z))
		{
			if (ccs.x > 0.f)
				m_phi = 90.f;
			else
				m_phi = 270.f;
		}
		else
		{
			float tanPhi{ 0.f };
			if (ccs.x >= 0.f && ccs.z > 0.f)
			{
				tanPhi = ccs.x / ccs.z;
				m_phi = glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x >= 0.f && ccs.z < 0.f)
			{
				tanPhi = ccs.x / std::abs(ccs.z);
				m_phi = 180.f - glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x <= 0.f && ccs.z < 0.f)
			{
				tanPhi = ccs.x / ccs.z;
				m_phi = 180.f + glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x <= 0.f && ccs.z > 0.f)
			{
				tanPhi = std::abs(ccs.x) / ccs.z;
				m_phi = 360.f - glm::degrees(std::atan(tanPhi));
			}
		}
	}

	glm::vec3 SphereCoord::toCartesian()
	{
		glm::vec3 out{ 0.f };
		float projection = std::abs(m_radius * std::sin(glm::radians(m_theta)));
		out.x = projection * std::sin(glm::radians(m_phi));
		out.y = m_radius * std::cos(glm::radians(m_theta));
		out.z = projection * std::cos(glm::radians(m_phi));
		return out;
	}

	void SphereCoord::checkRange()
	{
		// First, check theta's range
		if (m_theta > 179.f)
		{
			float exceed = m_theta - 179.f;
			m_theta = 179.f;
			m_phi += exceed;
		}
		else if (m_theta < 1.f)
		{
			float exceed = 1.f - m_theta;
			m_theta = 1.f;
			m_phi += exceed;
		}

		// Second, check phi's range
		if (m_phi > 360.f)
		{
			m_phi = (int)m_phi % 360;
		}
		else if (m_phi < 0.f)
		{
			float absPhi = std::abs(m_phi);
			absPhi = (int)absPhi % 360;
			m_phi = 360.f - absPhi;
		}
	}

	bool SphereCoord::test(float theta, float phi, float r)
	{
		return !MathLib::equalF(m_theta, theta) ||
			   !MathLib::equalF(m_phi, phi) ||
			   !MathLib::equalF(m_radius, r);
	}
}