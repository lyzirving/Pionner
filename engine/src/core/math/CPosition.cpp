#include "CPosition.h"

namespace pio
{
	CPosition& CPosition::operator=(const glm::vec3& pos)
	{
		if (m_cPos != pos)
		{
			m_cPos = pos;
			m_sPos = SphereCoord::ToSCS(pos);
		}
		return *this;
	}
	CPosition& CPosition::operator=(const SphereCoord& pos)
	{
		if (m_sPos != pos)
		{
			m_sPos = pos;
			m_cPos = SphereCoord::ToCCS(pos);
		}
		return *this;
	}

	CPosition CPosition::operator+(const glm::vec3& pos)
	{
		return CPosition(this->m_cPos + pos);
	}

	CPosition& CPosition::operator+=(const glm::vec3& pos)
	{
		this->m_cPos += pos;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator-(const glm::vec3& pos)
	{
		return CPosition(this->m_cPos - pos);
	}

	CPosition& CPosition::operator-=(const glm::vec3& pos)
	{
		this->m_cPos -= pos;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator*(const glm::vec3& pos)
	{
		return CPosition(this->m_cPos * pos);
	}

	CPosition& CPosition::operator*=(const glm::vec3& pos)
	{
		this->m_cPos *= pos;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator/(const glm::vec3& pos)
	{
		return CPosition(this->m_cPos / pos);
	}

	CPosition& CPosition::operator/=(const glm::vec3& pos)
	{
		this->m_cPos /= pos;
		this->m_sPos = SphereCoord::ToSCS(this->m_cPos);
		return *this;
	}

	CPosition CPosition::operator+(const SphereCoord& pos)
	{
		CPosition c;
		c.m_sPos = this->m_sPos + pos;
		c.m_cPos = SphereCoord::ToCCS(c.m_sPos);
		return c;
	}

	CPosition& CPosition::operator+=(const SphereCoord& pos)
	{
		this->m_sPos += pos;
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator-(const SphereCoord& pos)
	{
		CPosition c;
		c.m_sPos = this->m_sPos - pos;
		c.m_cPos = SphereCoord::ToCCS(c.m_sPos);
		return c;
	}

	CPosition& CPosition::operator-=(const SphereCoord& pos)
	{
		this->m_sPos -= pos;
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator*(const SphereCoord& pos)
	{
		CPosition c;
		c.m_sPos = this->m_sPos * pos;
		c.m_cPos = SphereCoord::ToCCS(c.m_sPos);
		return c;
	}

	CPosition& CPosition::operator*=(const SphereCoord& pos)
	{
		this->m_sPos *= pos;
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}

	CPosition CPosition::operator/(const SphereCoord& pos)
	{
		CPosition c;
		c.m_sPos = this->m_sPos / pos;
		c.m_cPos = SphereCoord::ToCCS(c.m_sPos);
		return c;
	}

	CPosition& CPosition::operator/=(const SphereCoord& pos)
	{
		this->m_sPos /= pos;
		this->m_cPos = SphereCoord::ToCCS(this->m_sPos);
		return *this;
	}
}