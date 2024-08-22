#ifndef __PIONNER_BASE_POSITION_3D_H__
#define __PIONNER_BASE_POSITION_3D_H__

#include "SphereCoord.h"

namespace pio
{
	class Position3d
	{
	public:
		Position3d() {}
		Position3d(const glm::vec3& pos) : m_cartesianPos(pos), m_sphericalPos(SphereCoord::ToSCS(pos)) {}
		Position3d(const SphereCoord& pos) : m_cartesianPos(SphereCoord::ToCCS(pos)), m_sphericalPos(pos) {}
		Position3d(const Position3d& rhs) : m_cartesianPos(rhs.m_cartesianPos), m_sphericalPos(rhs.m_sphericalPos) {}
		Position3d(Position3d&& rhs) noexcept : m_cartesianPos(std::move(rhs.m_cartesianPos)), m_sphericalPos(std::move(rhs.m_sphericalPos)) {}

		Position3d& operator=(const Position3d &rhs);
		Position3d& operator=(Position3d&& rhs) noexcept;

		// Position in Cartesian coordinate system
		glm::vec3& ccs() { return m_cartesianPos; }
		const glm::vec3& ccs() const { return m_cartesianPos; }
		// Position in Spherical coordinate system
		SphereCoord& scs() { return m_sphericalPos; }
		const SphereCoord& scs() const { return m_sphericalPos; }

		operator glm::vec3() const { return m_cartesianPos; }
		operator const glm::vec3& () const { return m_cartesianPos; }

		operator SphereCoord() const { return m_sphericalPos; }
		operator const SphereCoord &() const { return m_sphericalPos; }

		// flush based on Cartesian coordinate system
		void CFlush() { m_sphericalPos = SphereCoord::ToSCS(m_cartesianPos); }
		// flush based on Spherical coordinate system
		void SFlush() { m_cartesianPos = SphereCoord::ToCCS(m_sphericalPos); }

		Position3d& operator=(const glm::vec3& rhs);
		Position3d& operator=(const SphereCoord& rhs);

		Position3d  operator+(const glm::vec3& rhs);
		Position3d& operator+=(const glm::vec3& rhs);

		Position3d  operator-(const glm::vec3& rhs);
		Position3d& operator-=(const glm::vec3& rhs);

		Position3d  operator*(const glm::vec3& rhs);
		Position3d& operator*=(const glm::vec3& rhs);

		Position3d  operator/(const glm::vec3& rhs);
		Position3d& operator/=(const glm::vec3& rhs);

		Position3d  operator+(const SphereCoord& rhs);
		Position3d& operator+=(const SphereCoord& rhs);

		Position3d  operator-(const SphereCoord& rhs);
		Position3d& operator-=(const SphereCoord& rhs);

		Position3d  operator*(const SphereCoord& rhs);
		Position3d& operator*=(const SphereCoord& rhs);

		Position3d  operator/(const SphereCoord& rhs);
		Position3d& operator/=(const SphereCoord& rhs);

		Position3d  operator+(const Position3d& rhs);
		Position3d& operator+=(const Position3d& rhs);

		Position3d  operator-(const Position3d& rhs);
		Position3d& operator-=(const Position3d& rhs);

		Position3d  operator*(const Position3d& rhs);
		Position3d& operator*=(const Position3d& rhs);

		Position3d  operator/(const Position3d& rhs);
		Position3d& operator/=(const Position3d& rhs);

		bool operator==(const glm::vec3& rhs);
		bool operator!=(const glm::vec3& rhs);

		bool operator==(const SphereCoord &rhs);
		bool operator!=(const SphereCoord& rhs);

		bool operator==(const Position3d& rhs);
		bool operator!=(const Position3d& rhs);

	public:
		inline friend Position3d operator*(const glm::mat4& mat, const Position3d& p);

		inline friend Position3d operator+(const glm::vec3& lhs, const Position3d& p);
		inline friend Position3d operator-(const glm::vec3& lhs, const Position3d& p);
		inline friend Position3d operator*(const glm::vec3& lhs, const Position3d& p);
		inline friend Position3d operator/(const glm::vec3& lhs, const Position3d& p);

	private:
		glm::vec3 m_cartesianPos{ 0.f };//position in Cartesian system
		SphereCoord m_sphericalPos{};     //position in spherical system
	};

	Position3d operator*(const glm::mat4& mat, const Position3d& p)
	{
		Position3d pos;
		pos.m_cartesianPos = mat * glm::vec4(p.m_cartesianPos, 1.f);
		pos.m_sphericalPos = SphereCoord::ToSCS(pos.m_cartesianPos);
		return pos;
	}

	Position3d operator+(const glm::vec3& lhs, const Position3d& p)
	{
		Position3d pos;
		pos.m_cartesianPos = lhs + p.m_cartesianPos;
		pos.m_sphericalPos = SphereCoord::ToSCS(pos.m_cartesianPos);
		return pos;
	}

	Position3d operator-(const glm::vec3& lhs, const Position3d& p)
	{
		Position3d pos;
		pos.m_cartesianPos = lhs - p.m_cartesianPos;
		pos.m_sphericalPos = SphereCoord::ToSCS(pos.m_cartesianPos);
		return pos;
	}

	Position3d operator*(const glm::vec3& lhs, const Position3d& p)
	{
		Position3d pos;
		pos.m_cartesianPos = lhs * p.m_cartesianPos;
		pos.m_sphericalPos = SphereCoord::ToSCS(pos.m_cartesianPos);
		return pos;
	}

	Position3d operator/(const glm::vec3& lhs, const Position3d& p)
	{
		Position3d pos;
		pos.m_cartesianPos = lhs / p.m_cartesianPos;
		pos.m_sphericalPos = SphereCoord::ToSCS(pos.m_cartesianPos);
		return pos;
	}
}

#endif