#include "SphereCoord.h"

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

	bool SphereCoord::operator==(const SphereCoord &rhs)
	{
		if (this == &rhs)
			return true;

		return Math::Equal(m_theta, rhs.m_theta) && 
			   Math::Equal(m_phi, rhs.m_phi) &&
			   Math::Equal(m_radius, rhs.m_radius);
	}

	bool SphereCoord::operator==(const SphereCoord &rhs) const
	{
		if (this == &rhs)
			return true;

		return Math::Equal(m_theta, rhs.m_theta) &&
			   Math::Equal(m_phi, rhs.m_phi) &&
			   Math::Equal(m_radius, rhs.m_radius);
	}

	bool SphereCoord::operator!=(const SphereCoord &rhs)
	{
		return !((*this) == rhs);
	}

	bool SphereCoord::operator!=(const SphereCoord &rhs) const
	{
		return !((*this) == rhs);
	}

	void SphereCoord::applyCartesian(const glm::vec3 &ccs)
	{
		SphereCoord ret = SphereCoord::ToSCS(ccs);
		m_theta = ret.m_theta;
		m_phi = ret.m_phi;
		m_radius = ret.m_radius;
	}

	glm::vec3 SphereCoord::toCartesian() const
	{
		return SphereCoord::ToCCS(m_theta, m_phi, m_radius);
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

		if (m_radius < 1.f)
		{
			m_radius = 1.f;
		}
		else if (m_radius > 20.f)
		{
			m_radius = 20.f;
		}
	}

	bool SphereCoord::test(float theta, float phi, float r)
	{
		return !Math::Equal(m_theta, theta) ||
			   !Math::Equal(m_phi, phi) ||
			   !Math::Equal(m_radius, r);
	}

	glm::vec3 SphereCoord::ToCCS(float theta, float phi, float radius)
	{
		float projection = std::abs(radius * std::sin(glm::radians(theta)));
		return glm::vec3(projection * std::sin(glm::radians(phi)),
						 radius * std::cos(glm::radians(theta)),
						 projection * std::cos(glm::radians(phi)));
	}

	glm::vec3 SphereCoord::ToCCS(const SphereCoord &coord)
	{
		return ToCCS(coord.m_theta, coord.m_phi, coord.m_radius);
	}

	SphereCoord SphereCoord::ToSCS(const glm::vec3 &ccs)
	{
		SphereCoord result;

		if (Math::IsZero(ccs.z) && Math::IsZero(ccs.x) && Math::IsZero(ccs.y))
		{
			result.m_theta = 0.f;
			result.m_phi = 0.f;
			result.m_radius = 0.f;
			return result;
		}
		else if (Math::IsZero(ccs.z) && Math::IsZero(ccs.x))
		{
			result.m_theta = ccs.y > 0.f ? 0.f : 180.f;
			result.m_phi = 0.f;
			result.m_radius = std::abs(ccs.y);
			return result;
		}
		else if (Math::IsZero(ccs.z) && Math::IsZero(ccs.y))
		{
			result.m_theta = 90.f;
			result.m_phi = ccs.x > 0.f ? 90.f : 270.f;
			result.m_radius = std::abs(ccs.x);
			return result;
		}
		else if (Math::IsZero(ccs.x) && Math::IsZero(ccs.y))
		{
			result.m_theta = 90.f;
			result.m_phi = ccs.z > 0.f ? 0.f : 180.f;
			result.m_radius = std::abs(ccs.z);
			return result;
		}

		float len = std::sqrt(std::pow(ccs.x, 2) + std::pow(ccs.y, 2) + std::pow(ccs.z, 2));

		result.m_radius = len;
		result.m_theta = glm::degrees(std::acos(ccs.y / len));

		if (Math::IsZero(ccs.z))
		{
			if (ccs.x > 0.f)
				result.m_phi = 90.f;
			else
				result.m_phi = 270.f;
		}
		else
		{
			float tanPhi{ 0.f };
			if (ccs.x >= 0.f && ccs.z > 0.f)
			{
				tanPhi = ccs.x / ccs.z;
				result.m_phi = glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x >= 0.f && ccs.z < 0.f)
			{
				tanPhi = ccs.x / std::abs(ccs.z);
				result.m_phi = 180.f - glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x <= 0.f && ccs.z < 0.f)
			{
				tanPhi = ccs.x / ccs.z;
				result.m_phi = 180.f + glm::degrees(std::atan(tanPhi));
			}
			else if (ccs.x <= 0.f && ccs.z > 0.f)
			{
				tanPhi = std::abs(ccs.x) / ccs.z;
				result.m_phi = 360.f - glm::degrees(std::atan(tanPhi));
			}
		}

		return result;
	}
}