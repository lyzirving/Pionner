#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "CameraImpl.h"
#include "core/math/MathLib.h"

namespace pio
{
	namespace sgf
	{
		CameraImpl::CameraImpl()
		{
			m_dirty.set();
		}

		CameraImpl::~CameraImpl() = default;

		const glm::mat4 &CameraImpl::getViewMat()
		{
			calcViewMat();
			return m_viewMat;
		}

		const glm::vec3 &CameraImpl::getCamPos()
		{
			calcViewMat();
			return m_camPos;
		}

		void CameraImpl::setPosition(float theta, float phi, float r)
		{
			if (m_camPosSpherical.test(theta, phi, r))
			{
				m_camPosSpherical.set(theta, phi, r);
				m_camPosSpherical.checkRange();
				m_dirty.set(CAM_BIT_POS);
				m_dirty.set(CAM_BIT_VIEW_MAT);
			}
		}

		void CameraImpl::setPosition(const glm::vec3 &pos)
		{
			if (!MathLib::equalVec3(m_camPos, pos))
			{
				m_camPos = pos;
				m_camPosSpherical.applyCartesian(pos);
				m_dirty.set(CAM_BIT_VIEW_MAT);
			}
		}

		void CameraImpl::setLookAt(const glm::vec3 &lookAt)
		{
			if (!MathLib::equalVec3(m_lookAt, lookAt))
			{
				m_lookAt = lookAt;
				m_dirty.set(CAM_BIT_VIEW_MAT);
			}
		}

		void CameraImpl::calcViewMat()
		{
			if (m_dirty.test(CAM_BIT_VIEW_MAT))
			{
				glm::vec3 viewDir = glm::normalize(m_lookAt - m_camPos);
				// Test the bit to avoid invalid calculation.
				if (m_dirty.test(CAM_BIT_POS))
				{
					m_camPos = m_camPosSpherical.toCartesian();
				}

				// compute the right and up vector by view direction and world up
				m_camRight = glm::normalize(glm::cross(viewDir, glm::vec3(0.f, 1.f, 0.f)));
				m_camUp = glm::normalize(glm::cross(m_camRight, viewDir));

				m_viewMat = glm::lookAt(m_camPos, m_lookAt, m_camUp);

				m_dirty.reset();
			}
		}
	}
}