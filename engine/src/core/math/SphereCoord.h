#ifndef __PIONNER_CORE_MATH_SPHERE_COORD_H__
#define __PIONNER_CORE_MATH_SPHERE_COORD_H__

#include "core/Base.h"

namespace pio
{
	/**
	* @brief: Sphere coordinate system is a right-handed coordinate system.
	*		  It is formed by three components: theta, phi and radius.
	*         Assume there is a Cartesian coordinate system x-y-z, and x-y-z is a right-handed coordinate whose origin is O.
	*         And there is also a point P in x-y-z.
	*		  Theta is a polar angle between +y axis and vector OP.
	*		  Phi is an azimuth between +z and projected vector of P on plane x-z.
	*		  Radius specifies OP's absolute length.
	*		  Overall, theta, phi and radius can locate a point in x-y-z.
	*/
	class SphereCoord
	{
	public:
		SphereCoord() {}
		SphereCoord(float theta, float phi, float radius) : m_theta(theta), m_phi(phi), m_radius(radius) {}
		~SphereCoord() = default;

		bool operator==(const SphereCoord &rhs);
		bool operator==(const SphereCoord &rhs) const;

		bool operator!=(const SphereCoord &rhs);
		bool operator!=(const SphereCoord &rhs) const;

		SphereCoord operator+(const SphereCoord& rhs);
		SphereCoord &operator+=(const SphereCoord& rhs);

		SphereCoord operator-(const SphereCoord& rhs);
		SphereCoord& operator-=(const SphereCoord& rhs);

		SphereCoord operator*(const SphereCoord& rhs);
		SphereCoord& operator*=(const SphereCoord& rhs);

		SphereCoord operator/(const SphereCoord& rhs);
		SphereCoord& operator/=(const SphereCoord& rhs);

		void setTheta(float val)  { m_theta = val; }
		void setPhi(float val)    { m_phi = val; }
		void setRadius(float val) { m_radius = val; }
		void set(float theta, float phi, float r) { m_theta = theta; m_phi = phi; m_radius = r; }

		float getTheta() const  { return m_theta; }
		float getPhi() const    { return m_phi; }
		float getRadius() const { return m_radius; }

		float *ptrTheta()  { return &m_theta; }
		float *ptrPhi() { return &m_phi; }
		float *ptrRadius() { return &m_radius; }

		/*
		* @brief     Apply input position
		* @param ccs Input position based on cartesian coordinate system.
		*/
		void apply(const glm::vec3 &ccs);
		/*
		* @brief   Transform spherical position to cartesian coordinate system.
		* @return  Position in  cartesian coordinate system
		*/
		glm::vec3 to() const;
		void checkRange();

	public:
		// Transform spherical coordinate system to cartesian.
		static glm::vec3 ToCCS(float theta, float phi, float radius);
		static glm::vec3 ToCCS(const SphereCoord& coord);
		// Transform cartesian coordinate system to spherical.
		static SphereCoord ToSCS(const glm::vec3 &ccs);

	private:
		float m_theta{ 0.f }, m_phi{ 0.f }, m_radius{ 0.f };
	};
}

#endif