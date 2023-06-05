#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "function/render/scene/Camera.h"

#include "core/math/MathLib.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

namespace Pionner
{
	Camera::Camera(float theta, float phi, float radius, float viewTheta, float viewPhi)
		: m_worldUp(0.f, 1.f, 0.f)
		, m_camPos(0.f)
		, m_camRight(0.f), m_camUp(0.f), m_viewDir(0.f)
		, m_theta(theta), m_phi(phi), m_radius(radius)
		, m_viewTheta(viewTheta), m_viewPhi(viewPhi)
		, m_viewMat(1.f), m_viewInvMat(1.f)
		, m_dataChange(true)
		, m_stateStack()
	{
		// compute view direction at first
		// camera always look at the origin of world by now
		calcViewDirection();
		calcCameraPosition();
	}

	Camera::~Camera()
	{
	}

	const glm::mat4 &Camera::getViewMat()
	{
		calcViewMat();
		return m_viewMat;
	}

	const glm::mat4 &Camera::getViewInvMat()
	{
		calcViewMat();
		return m_viewInvMat;
	}

	const glm::vec3 &Camera::getCamPos()
	{
		calcViewMat();
		return m_camPos;
	}

	void Camera::dealScrollPosition(float deltaX, float deltaY)
	{
		//LOG_DEBUG("delta[%f, %f]", deltaX, deltaY);
	}

	void Camera::setPosition(float theta, float phi, float r)
	{
		bool equal = MathLib::equalF(m_theta, theta) && MathLib::equalF(m_phi, phi)
			&& MathLib::equalF(m_radius, r);
		if (!equal)
		{
			m_theta = theta;
			m_phi = phi;
			m_radius = r;
			checkCamPos();
			m_dataChange.store(true);
		}
	}

	void Camera::setPosition(const glm::vec3 &pos)
	{
		if (!MathLib::equalVec3(m_camPos, pos))
		{
			glm::vec3 camPosSCS;
			MathLib::CCStoSCS(pos, camPosSCS);
			m_theta = camPosSCS.x;
			m_phi = camPosSCS.y;
			m_radius = camPosSCS.z;
			checkCamPos();
			m_dataChange.store(true);
		}
	}

	void Camera::setLookAt(const glm::vec3 &lookAt)
	{
		// need to be implemented
		m_viewDir = glm::normalize(lookAt - m_camPos);
		glm::vec3 out;
		MathLib::CCStoSCS(m_viewDir, out);
		m_viewTheta = out.x;
		m_viewPhi = out.y;
		m_dataChange.store(true);
	}

	void Camera::restoreState()
	{
		CameraState state{ m_theta, m_phi, m_radius, m_viewTheta, m_viewPhi };
		m_stateStack.push_back(state);
	}

	void Camera::popState()
	{
		if (m_stateStack.empty())
		{
			LOG_ERR("state stack is empty, err operation");
			return;
		}
		CameraState &state = m_stateStack.front();
		m_theta = state.m_theta;
		m_phi = state.m_phi;
		m_radius = state.m_radius;
		m_viewTheta = state.m_viewTheta;
		m_viewPhi = state.m_viewPhi;
		calcCameraPosition();
		calcViewDirection();
		m_dataChange.store(true);

		m_stateStack.pop_front();
	}

	void Camera::calcViewMat()
	{
		if (m_dataChange.load())
		{
			// compute the right and up vector by view direction and world up
			m_camRight = glm::normalize(glm::cross(m_viewDir, m_worldUp));
			m_camUp = glm::normalize(glm::cross(m_camRight, m_viewDir));

			m_viewMat = glm::lookAt(m_camPos, m_camPos + m_viewDir, m_camUp);
			m_viewInvMat = glm::inverse(m_viewMat);

			m_dataChange.store(false);
		}
	}

	void Camera::calcViewDirection()
	{
		MathLib::SCStoCCS(m_viewTheta, m_viewPhi, 1.f, m_viewDir);
	}

	void Camera::calcCameraPosition()
	{
		MathLib::SCStoCCS(m_theta, m_phi, m_radius, m_camPos);
	}

	void Camera::checkCamPos()
	{
		checkCamPosPhi();

		float absTheta = std::abs(m_theta);
		bool odd = (((int)absTheta / 180) % 2) != 0;

		if (m_theta > 180.f)
		{
			if (odd)
			{
				m_theta = 180.f - ((int)absTheta % 180);
				m_phi += 180.f;
				checkCamPosPhi();
			}
			else
			{
				m_theta = (float)((int)absTheta % 180);
			}
		}
		else if (m_theta < 0.f)
		{
			if (odd)
			{
				m_theta = 180.f - ((int)(absTheta) % 180);
			}
			else
			{
				m_theta = (float)((int)(absTheta) % 180);
				m_phi += 180.f;
				checkCamPosPhi();
			}
		}

		calcCameraPosition();
		keepViewDirToOrigin();
	}

	void Camera::checkCamPosPhi()
	{
		if (m_phi > 360.f)
		{
			m_phi = (int)m_phi % 360;
		}
		else if (m_phi < 0.f)
		{
			float absPhi = std::abs(m_phi);
			absPhi = (int)absPhi % 360;
			m_phi = 360.f - absPhi;
		}
	}

	void Camera::keepViewDirToOrigin()
	{
		glm::vec3 viewDir, viewDirSCS;
		viewDir = glm::normalize(-m_camPos);
		MathLib::CCStoSCS(viewDir, viewDirSCS);

		// get theta and phi of view direction
		m_viewTheta = viewDirSCS.x;
		m_viewPhi = viewDirSCS.y;

		calcViewDirection();
	}
}