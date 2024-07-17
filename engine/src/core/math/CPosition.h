#ifndef __PIONNER_CORE_MATH_CPOSITION_H__
#define __PIONNER_CORE_MATH_CPOSITION_H__

#include "SphereCoord.h"

namespace pio
{
	class Transform;

	class CPosition
	{
	public:
		CPosition() {}
		CPosition(const glm::vec3& pos) : m_cPos(pos), m_sPos(SphereCoord::ToSCS(pos)) {}
		CPosition(const SphereCoord& pos) : m_cPos(SphereCoord::ToCCS(pos)), m_sPos(pos) {}
		CPosition(const CPosition& rhs) : m_cPos(rhs.m_cPos), m_sPos(rhs.m_sPos) {}
		CPosition(CPosition&& rhs) noexcept : m_cPos(std::move(rhs.m_cPos)), m_sPos(std::move(rhs.m_sPos)) {}

		CPosition& operator=(const CPosition &rhs);
		CPosition& operator=(CPosition&& rhs) noexcept;

		// Position in Cartesian coordinate system
		glm::vec3& ccs() { return m_cPos; }
		const glm::vec3& ccs() const { return m_cPos; }
		// Position in Spherical coordinate system
		SphereCoord& scs() { return m_sPos; }
		const SphereCoord& scs() const { return m_sPos; }

		operator glm::vec3() const { return m_cPos; }
		operator const glm::vec3& () const { return m_cPos; }

		operator SphereCoord() const { return m_sPos; }
		operator const SphereCoord &() const { return m_sPos; }

		// flush based on Cartesian coordinate system
		void CFlush() { m_sPos = SphereCoord::ToSCS(m_cPos); }
		// flush based on Spherical coordinate system
		void SFlush() { m_cPos = SphereCoord::ToCCS(m_sPos); }

		CPosition& operator=(const glm::vec3& rhs);
		CPosition& operator=(const SphereCoord& rhs);

		CPosition  operator+(const glm::vec3& rhs);
		CPosition& operator+=(const glm::vec3& rhs);

		CPosition  operator-(const glm::vec3& rhs);
		CPosition& operator-=(const glm::vec3& rhs);

		CPosition  operator*(const glm::vec3& rhs);
		CPosition& operator*=(const glm::vec3& rhs);

		CPosition  operator/(const glm::vec3& rhs);
		CPosition& operator/=(const glm::vec3& rhs);

		CPosition  operator+(const SphereCoord& rhs);
		CPosition& operator+=(const SphereCoord& rhs);

		CPosition  operator-(const SphereCoord& rhs);
		CPosition& operator-=(const SphereCoord& rhs);

		CPosition  operator*(const SphereCoord& rhs);
		CPosition& operator*=(const SphereCoord& rhs);

		CPosition  operator/(const SphereCoord& rhs);
		CPosition& operator/=(const SphereCoord& rhs);

		CPosition  operator+(const CPosition& rhs);
		CPosition& operator+=(const CPosition& rhs);

		CPosition  operator-(const CPosition& rhs);
		CPosition& operator-=(const CPosition& rhs);

		CPosition  operator*(const CPosition& rhs);
		CPosition& operator*=(const CPosition& rhs);

		CPosition  operator/(const CPosition& rhs);
		CPosition& operator/=(const CPosition& rhs);

		bool operator==(const glm::vec3& rhs);
		bool operator!=(const glm::vec3& rhs);

		bool operator==(const SphereCoord &rhs);
		bool operator!=(const SphereCoord& rhs);

		bool operator==(const CPosition& rhs);
		bool operator!=(const CPosition& rhs);

	public:
		inline friend CPosition operator*(const glm::mat4& mat, const CPosition& p);

		inline friend CPosition operator+(const glm::vec3& lhs, const CPosition& p);
		inline friend CPosition operator-(const glm::vec3& lhs, const CPosition& p);
		inline friend CPosition operator*(const glm::vec3& lhs, const CPosition& p);
		inline friend CPosition operator/(const glm::vec3& lhs, const CPosition& p);

	private:
		glm::vec3   m_cPos{ 0.f };//position in Cartesian system
		SphereCoord m_sPos{};     //position in spherical system
	};

	CPosition operator*(const glm::mat4& mat, const CPosition& p)
	{
		CPosition pos;
		pos.m_cPos = mat * glm::vec4(p.m_cPos, 1.f);
		pos.m_sPos = SphereCoord::ToSCS(pos.m_cPos);
		return pos;
	}

	CPosition operator+(const glm::vec3& lhs, const CPosition& p)
	{
		CPosition pos;
		pos.m_cPos = lhs + p.m_cPos;
		pos.m_sPos = SphereCoord::ToSCS(pos.m_cPos);
		return pos;
	}

	CPosition operator-(const glm::vec3& lhs, const CPosition& p)
	{
		CPosition pos;
		pos.m_cPos = lhs - p.m_cPos;
		pos.m_sPos = SphereCoord::ToSCS(pos.m_cPos);
		return pos;
	}

	CPosition operator*(const glm::vec3& lhs, const CPosition& p)
	{
		CPosition pos;
		pos.m_cPos = lhs * p.m_cPos;
		pos.m_sPos = SphereCoord::ToSCS(pos.m_cPos);
		return pos;
	}

	CPosition operator/(const glm::vec3& lhs, const CPosition& p)
	{
		CPosition pos;
		pos.m_cPos = lhs / p.m_cPos;
		pos.m_sPos = SphereCoord::ToSCS(pos.m_cPos);
		return pos;
	}
}

#endif