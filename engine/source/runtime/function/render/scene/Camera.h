#ifndef __RENDER_CAMERA_H__
#define __RENDER_CAMERA_H__

#include <glm/glm.hpp>
#include <atomic>
#include <list>

namespace Pionner
{
	class Camera
	{
	public:
		Camera(float theta = 90.f, float phi = 0.f, float radius = 5.f, float viewTheta = 90.f, float viewPhi = 180.f);
		~Camera();

		inline bool isChanged() { return m_dataChange.load(); }

		const glm::mat4 &getViewMat();
		const glm::mat4 &getViewInvMat();
		const glm::vec3 &getCamPos();

		void setPosition(float theta, float phi, float r);
		void setPosition(const glm::vec3 &pos);
		void setLookAt(const glm::vec3 &lookAt);

		void restoreState();
		void popState();

	private:
		struct CameraState
		{
			float m_theta, m_phi;
			float m_radius;
			float m_viewTheta, m_viewPhi;

			CameraState() : m_theta(0.f), m_phi(0.f), m_radius(0.f), m_viewTheta(0.f), m_viewPhi(0.f)
			{}

			CameraState(float posTheta, float posPhi, float posRadius, float viewTheta, float viewPhi)
				: m_theta(posTheta), m_phi(posPhi), m_radius(posRadius)
				, m_viewTheta(viewTheta), m_viewPhi(viewPhi)
			{}
		};

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
		glm::mat4 m_viewInvMat;
		std::atomic<bool> m_dataChange;

		std::list<CameraState> m_stateStack;
	};
}

#endif