#include "Camera2.h"

namespace pio
{
	void Camera2::flush()
	{
		calcViewMat();
		m_frustum.calcPrj();
		m_frustum.calcOrtho();
	}

	void Camera2::setPosition(const glm::vec3& position)
	{
		if (m_transform.Position != position)
		{
			m_transform.Position = position;
			m_dirty = true;
		}
	}

	void Camera2::setPosition(const SphereCoord& position)
	{
	}

	void Camera2::setLookAt(const glm::vec3& lookAt)
	{
		if (m_pose.LookAt != lookAt)
		{
			m_pose.LookAt = lookAt;
			calcCameraPose();	
			// camera pose axis -> rotation matrix -> quaternion -> euler angle
			glm::quat quat = glm::toQuat(glm::transpose(glm::mat3(m_pose.Right, m_pose.Up, m_pose.Front)));
			m_transform.Euler = glm::degrees(glm::eulerAngles(quat));
			m_dirty = true;
		}
	}

	void Camera2::calcViewMat()
	{
		if (m_dirty)
		{			
			m_pose.ViewMat = glm::lookAt(m_transform.Position.ccs(), m_pose.LookAt, m_pose.Up);
			m_dirty = false;
		}
	}

	void Camera2::calcCameraPose()
	{
		glm::vec3 viewDir = glm::normalize(m_pose.LookAt - m_transform.Position.ccs());
		// compute the right and up vector by view direction and world up
		m_pose.Right = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
		m_pose.Up = glm::normalize(glm::cross(m_pose.Right, viewDir));
		m_pose.Front = -viewDir;
	}
}