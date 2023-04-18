#ifndef __RENDER_CAMERA_H__
#define __RENDER_CAMERA_H__

#include <glm/glm.hpp>
#include <atomic>

namespace Pionner
{
	class Camera
	{
	public:
		Camera(float theta = 90.f, float phi = 0.f, float radius = 5.f, float viewTheta = 90.f, float viewPhi = 180.f);
		~Camera();

		inline bool isChanged() { return m_dataChange.load(); }

		const glm::mat4 &getViewMat();
		const glm::vec3 &getCamPos();

		void setPosition(float theta, float phi, float r);
		void setPosition(const glm::vec3 &pos);
		void setLookAt(const glm::vec3 &lookAt);

	private:
		void calcViewMat();
		void calcViewDirection();
		void calcCameraPosition();
		void checkCamPos();
		void checkCamPosPhi();
		void keepViewDirToOrigin();

	private:
		glm::vec3 m_worldUp;
		glm::vec3 m_camPos;
		// +x axis
		glm::vec3 m_camRight;
		// +y axis
		glm::vec3 m_camUp;
		// -z axis
		glm::vec3 m_viewDir;
		// (theta, phi, r) spherical coordinate based on right handed coordinate system.
		// theta is polar angle between +y axis and vector OC, while O is origin, C is camera position.
		float m_theta;
		// (theta, phi, r) spherical coordinate based on right handed coordinate system.
		// phi is azimuth between +z and projected vector of camera position on plane x-z.
		float m_phi;
		// (theta, phi, r) spherical coordinate based on right handed coordinate system.
		// r specifies camera position's absolute length
		float m_radius;
		// spherical coordinate of view sight direction. r for view direction is 1.0.
		float m_viewTheta, m_viewPhi;

		glm::mat4 m_viewMat;
		std::atomic<bool> m_dataChange;
	};
}

#endif