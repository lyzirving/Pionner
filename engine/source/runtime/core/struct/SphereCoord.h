#ifndef __PIONNER_CORE_SPHERE_COORD_H__
#define __PIONNER_CORE_SPHERE_COORD_H__

#include <glm/glm.hpp>

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
		SphereCoord();
		SphereCoord(float theta, float phi, float radius);
		~SphereCoord();

		inline void setTheta(float val)  { m_theta = val; }
		inline void setPhi(float val)    { m_phi = val; }
		inline void setRadius(float val) { m_radius = val; }
		inline void set(float theta, float phi, float r)
		{
			m_theta = theta;
			m_phi = phi;
			m_radius = r;
		}

		inline float getTheta() const  { return m_theta; }
		inline float getPhi() const    { return m_phi; }
		inline float getRadius() const { return m_radius; }

		/*
		* @brief: Transform input cartesian coordinate into Sphere coordinate
		* @param ccs: input position based on cartesian coordinate system.
		*/
		void      applyCartesian(const glm::vec3 &ccs);
		glm::vec3 toCartesian();

		void      checkRange();
		bool      test(float theta, float phi, float r);

	private:
		float m_theta{ 0.f }, m_phi{ 0.f }, m_radius{ 0.f };
	};
}

#endif