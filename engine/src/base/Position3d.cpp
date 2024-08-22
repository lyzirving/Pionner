#include "Position3d.h"

namespace pio
{
	Position3d& Position3d::operator=(const Position3d& rhs)
	{
		if (this != &rhs)
		{
			m_cartesianPos = rhs.m_cartesianPos;
			m_sphericalPos = rhs.m_sphericalPos;
		}
		return *this;
	}

	Position3d& Position3d::operator=(Position3d&& rhs) noexcept
	{
		if (this != &rhs)
		{
			m_cartesianPos = std::move(rhs.m_cartesianPos);
			m_sphericalPos = std::move(rhs.m_sphericalPos);
		}
		return *this;
	}

	Position3d& Position3d::operator=(const glm::vec3& rhs)
	{
		if (m_cartesianPos != rhs)
		{
			m_cartesianPos = rhs;
			m_sphericalPos = SphereCoord::ToSCS(rhs);
		}
		return *this;
	}

	Position3d& Position3d::operator=(const SphereCoord& rhs)
	{
		if (m_sphericalPos != rhs)
		{
			m_sphericalPos = rhs;
			m_cartesianPos = SphereCoord::ToCCS(rhs);
		}
		return *this;
	}

	Position3d Position3d::operator+(const glm::vec3& rhs)
	{
		return Position3d(this->m_cartesianPos + rhs);
	}

	Position3d& Position3d::operator+=(const glm::vec3& rhs)
	{
		this->m_cartesianPos += rhs;
		this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		return *this;
	}

	Position3d Position3d::operator-(const glm::vec3& rhs)
	{
		return Position3d(this->m_cartesianPos - rhs);
	}

	Position3d& Position3d::operator-=(const glm::vec3& rhs)
	{
		this->m_cartesianPos -= rhs;
		this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		return *this;
	}

	Position3d Position3d::operator*(const glm::vec3& rhs)
	{
		return Position3d(this->m_cartesianPos * rhs);
	}

	Position3d& Position3d::operator*=(const glm::vec3& rhs)
	{
		this->m_cartesianPos *= rhs;
		this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		return *this;
	}

	Position3d Position3d::operator/(const glm::vec3& rhs)
	{
		return Position3d(this->m_cartesianPos / rhs);
	}

	Position3d& Position3d::operator/=(const glm::vec3& rhs)
	{
		this->m_cartesianPos /= rhs;
		this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		return *this;
	}

	Position3d Position3d::operator+(const SphereCoord& rhs)
	{
		Position3d p;
		p.m_sphericalPos = this->m_sphericalPos + rhs;
		p.m_sphericalPos.checkRange();
		p.m_cartesianPos = SphereCoord::ToCCS(p.m_sphericalPos);
		return p;
	}

	Position3d& Position3d::operator+=(const SphereCoord& rhs)
	{
		this->m_sphericalPos += rhs;
		this->m_sphericalPos.checkRange();
		this->m_cartesianPos = SphereCoord::ToCCS(this->m_sphericalPos);
		return *this;
	}

	Position3d Position3d::operator-(const SphereCoord& rhs)
	{
		Position3d p;
		p.m_sphericalPos = this->m_sphericalPos - rhs;
		p.m_sphericalPos.checkRange();
		p.m_cartesianPos = SphereCoord::ToCCS(p.m_sphericalPos);
		return p;
	}

	Position3d& Position3d::operator-=(const SphereCoord& rhs)
	{
		this->m_sphericalPos -= rhs;
		this->m_sphericalPos.checkRange();
		this->m_cartesianPos = SphereCoord::ToCCS(this->m_sphericalPos);
		return *this;
	}

	Position3d Position3d::operator*(const SphereCoord& rhs)
	{
		Position3d p;
		p.m_sphericalPos = this->m_sphericalPos * rhs;
		p.m_sphericalPos.checkRange();
		p.m_cartesianPos = SphereCoord::ToCCS(p.m_sphericalPos);
		return p;
	}

	Position3d& Position3d::operator*=(const SphereCoord& rhs)
	{
		this->m_sphericalPos *= rhs;
		this->m_sphericalPos.checkRange();
		this->m_cartesianPos = SphereCoord::ToCCS(this->m_sphericalPos);
		return *this;
	}

	Position3d Position3d::operator/(const SphereCoord& rhs)
	{
		Position3d p;
		p.m_sphericalPos = this->m_sphericalPos / rhs;
		p.m_sphericalPos.checkRange();
		p.m_cartesianPos = SphereCoord::ToCCS(p.m_sphericalPos);
		return p;
	}

	Position3d& Position3d::operator/=(const SphereCoord& rhs)
	{
		this->m_sphericalPos /= rhs;
		this->m_sphericalPos.checkRange();
		this->m_cartesianPos = SphereCoord::ToCCS(this->m_sphericalPos);
		return *this;
	}

	Position3d Position3d::operator+(const Position3d& rhs)
	{
		return Position3d(this->m_cartesianPos + rhs.m_cartesianPos);
	}

	Position3d& Position3d::operator+=(const Position3d& rhs)
	{
		if (this != &rhs)
		{
			this->m_cartesianPos += rhs.m_cartesianPos;
			this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		}
		return *this;
	}

	Position3d Position3d::operator-(const Position3d& rhs)
	{
		return Position3d(this->m_cartesianPos - rhs.m_cartesianPos);
	}

	Position3d& Position3d::operator-=(const Position3d& rhs)
	{
		if (this != &rhs)
		{
			this->m_cartesianPos -= rhs.m_cartesianPos;
			this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		}
		return *this;
	}

	Position3d Position3d::operator*(const Position3d& rhs)
	{
		return Position3d(this->m_cartesianPos * rhs.m_cartesianPos);
	}

	Position3d& Position3d::operator*=(const Position3d& rhs)
	{
		if (this != &rhs)
		{
			this->m_cartesianPos *= rhs.m_cartesianPos;
			this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		}
		return *this;
	}

	Position3d Position3d::operator/(const Position3d& rhs)
	{
		return Position3d(this->m_cartesianPos / rhs.m_cartesianPos);
	}

	Position3d& Position3d::operator/=(const Position3d& rhs)
	{
		if (this != &rhs)
		{
			this->m_cartesianPos /= rhs.m_cartesianPos;
			this->m_sphericalPos = SphereCoord::ToSCS(this->m_cartesianPos);
		}
		return *this;
	}

	bool Position3d::operator==(const glm::vec3& rhs)
	{
		return this->m_cartesianPos == rhs;
	}

	bool Position3d::operator!=(const glm::vec3& rhs)
	{
		return !((*this) == rhs);
	}

	bool Position3d::operator==(const SphereCoord& rhs)
	{
		return this->m_sphericalPos == rhs;
	}

	bool Position3d::operator!=(const SphereCoord& rhs)
	{
		return !((*this) == rhs);
	}

	bool Position3d::operator==(const Position3d& rhs)
	{
		return this->m_cartesianPos == rhs.m_cartesianPos;
	}

	bool Position3d::operator!=(const Position3d& rhs)
	{
		return this->m_cartesianPos != rhs.m_cartesianPos;
	}
}