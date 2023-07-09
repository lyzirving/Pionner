#include "Camera.h"

namespace pio
{
	namespace sgf
	{
		Camera::Camera(CameraId id) : m_id(id)
		{
		}

		Camera::~Camera() = default;

		void Camera::setPosition(float theta, float phi, float r)
		{
			m_camImpl.setPosition(theta, phi, r);
		}

		void Camera::setPosition(const glm::vec3 &pos)
		{
			m_camImpl.setPosition(pos);
		}

		void Camera::setLookAt(const glm::vec3 &lookAt)
		{
			m_camImpl.setLookAt(lookAt);
		}

		void Camera::setFov(float fov)
		{
			m_frustum.setFov(fov);
		}

		void Camera::setAspect(float aspect)
		{
			m_frustum.setAspect(aspect);
		}

		void Camera::setNearFar(float near, float far)
		{
			m_frustum.setNearFar(near, far);
		}

		void Camera::setOrtho(float l, float r, float b, float t)
		{
			m_frustum.setOrtho(l, r, b, t);
		}

		const glm::mat4 &Camera::getViewMat()
		{
			return m_camImpl.getViewMat();
		}

		const glm::mat4 &Camera::getPrjMat()
		{
			return m_frustum.getPerspectMat();
		}

		const glm::mat4 &Camera::getOrthoMat()
		{
			return m_frustum.getOrthoMat();
		}

		const glm::vec3 &Camera::getCamPos()
		{
			return m_camImpl.getCamPos();
		}
	}
}