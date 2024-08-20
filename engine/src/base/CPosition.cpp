#include "CPosition.h"

namespace pio
{
	CPosition& CPosition::operator=(const CPosition& rhs)
	{
		if (this != &rhs)
		{
			m_cPos = rhs.m_cPos;
			m_sPos = rhs.m_sPos;
		}
		return *this;
	}

	CPosition& CPosition::operator=(CPosition&& rhs) noexcept
	{
		if (this != &rhs)
		{
			m_cPos = std::move(rhs.m_cPos);
			m_sPos = std::move(rhs.m_sPos);
		}
		return *this;
	}

	CPosition& CPosition::operator=(const glm::vec3& rhs)
	{
		if (m_cPos != rhs)
		{
			m_cPos = rhs;
			m_sPos = SphereCoord::ToSCS(rhs);
		}
		return *this;
	}

	CPosition& CPosition::operator=(const SphereCoord& rhs)
	{
		if (m_sPos != rhs)
		{
			m_sPos = rhs;
			m_cPos = SphereCoord::ToCCS(rhs);
		}
		return *this;
	}

	CPosition CPosition::operator+(const glm::vec3& rhs)
	{
		return CPosition(this->m_cPos + rhs);
	}

	CPosition& CPosition::operator+=(const glm::vec3& rhs)
	{
		this->m_cPos += rhs;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator-(const glm::vec3& rhs)
	{
		return CPosition(this->m_cPos - rhs);
	}

	CPosition& CPosition::operator-=(const glm::vec3& rhs)
	{
		this->m_cPos -= rhs;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator*(const glm::vec3& rhs)
	{
		return CPosition(this->m_cPos * rhs);
	}

	CPosition& CPosition::operator*=(const glm::vec3& rhs)
	{
		this->m_cPos *= rhs;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator/(const glm::vec3& rhs)
	{
		return CPosition(this->m_cPos / rhs);
	}

	CPosition& CPosition::operator/=(const glm::vec3& rhs)
	{
		this->m_cPos /= rhs;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator+(const SphereCoord& rhs)
	{
		CPosition p;
		p.m_sPos = this->m_sPos + rhs;
		p.m_sPos.checkRange();
		p.m_cPos = SphereCoord::ToCCS(p.m_sPos);
		return p;
	}

	CPosition& CPosition::operator+=(const SphereCoord& rhs)
	{
		this->m_sPos += rhs;
		this->m_sPos.checkRange();
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator-(const SphereCoord& rhs)
	{
		CPosition p;
		p.m_sPos = this->m_sPos - rhs;
		p.m_sPos.checkRange();
		p.m_cPos = SphereCoord::ToCCS(p.m_sPos);
		return p;
	}

	CPosition& CPosition::operator-=(const SphereCoord& rhs)
	{
		this->m_sPos -= rhs;
		this->m_sPos.checkRange();
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator*(const SphereCoord& rhs)
	{
		CPosition p;
		p.m_sPos = this->m_sPos * rhs;
		p.m_sPos.checkRange();
		p.m_cPos = SphereCoord::ToCCS(p.m_sPos);
		return p;
	}

	CPosition& CPosition::operator*=(const SphereCoord& rhs)
	{
		this->m_sPos *= rhs;
		this->m_sPos.checkRange();
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator/(const SphereCoord& rhs)
	{
		CPosition p;
		p.m_sPos = this->m_sPos / rhs;
		p.m_sPos.checkRange();
		p.m_cPos = SphereCoord::ToCCS(p.m_sPos);
		return p;
	}

	CPosition& CPosition::operator/=(const SphereCoord& rhs)
	{
		this->m_sPos /= rhs;
		this->m_sPos.checkRange();
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator+(const CPosition& rhs)
	{
		return CPosition(this->m_cPos + rhs.m_cPos);
	}

	CPosition& CPosition::operator+=(const CPosition& rhs)
	{
		if (this != &rhs)
		{
			this->m_cPos += rhs.m_cPos;
			this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		}
		return *this;
	}

	CPosition CPosition::operator-(const CPosition& rhs)
	{
		return CPosition(this->m_cPos - rhs.m_cPos);
	}

	CPosition& CPosition::operator-=(const CPosition& rhs)
	{
		if (this != &rhs)
		{
			this->m_cPos -= rhs.m_cPos;
			this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		}
		return *this;
	}

	CPosition CPosition::operator*(const CPosition& rhs)
	{
		return CPosition(this->m_cPos * rhs.m_cPos);
	}

	CPosition& CPosition::operator*=(const CPosition& rhs)
	{
		if (this != &rhs)
		{
			this->m_cPos *= rhs.m_cPos;
			this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		}
		return *this;
	}

	CPosition CPosition::operator/(const CPosition& rhs)
	{
		return CPosition(this->m_cPos / rhs.m_cPos);
	}

	CPosition& CPosition::operator/=(const CPosition& rhs)
	{
		if (this != &rhs)
		{
			this->m_cPos /= rhs.m_cPos;
			this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		}
		return *this;
	}

	bool CPosition::operator==(const glm::vec3& rhs)
	{
		return this->m_cPos == rhs;
	}

	bool CPosition::operator!=(const glm::vec3& rhs)
	{
		return !((*this) == rhs);
	}

	bool CPosition::operator==(const SphereCoord& rhs)
	{
		return this->m_sPos == rhs;
	}

	bool CPosition::operator!=(const SphereCoord& rhs)
	{
		return !((*this) == rhs);
	}

	bool CPosition::operator==(const CPosition& rhs)
	{
		return this->m_cPos == rhs.m_cPos;
	}

	bool CPosition::operator!=(const CPosition& rhs)
	{
		return this->m_cPos != rhs.m_cPos;
	}
}