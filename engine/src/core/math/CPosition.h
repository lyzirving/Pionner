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

		const glm::vec3& ccs() const { return m_cPos; }
		const SphereCoord& scs() const { return m_sPos; }

		CPosition& operator=(const glm::vec3& pos);
		CPosition& operator=(const SphereCoord& pos);

		CPosition  operator+(const glm::vec3& pos);
		CPosition& operator+=(const glm::vec3& pos);

		CPosition  operator-(const glm::vec3& pos);
		CPosition& operator-=(const glm::vec3& pos);

		CPosition  operator*(const glm::vec3& pos);
		CPosition& operator*=(const glm::vec3& pos);

		CPosition  operator/(const glm::vec3& pos);
		CPosition& operator/=(const glm::vec3& pos);

		CPosition  operator+(const SphereCoord& pos);
		CPosition& operator+=(const SphereCoord& pos);

		CPosition  operator-(const SphereCoord& pos);
		CPosition& operator-=(const SphereCoord& pos);

		CPosition  operator*(const SphereCoord& pos);
		CPosition& operator*=(const SphereCoord& pos);

		CPosition  operator/(const SphereCoord& pos);
		CPosition& operator/=(const SphereCoord& pos);

	private:
		glm::vec3   m_cPos{ 0.f };//position in Cartesian system
		SphereCoord m_sPos{};     //position in spherical system

	private:
		friend class Transform;
	};
}

#endif