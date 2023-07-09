#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "Frustum.h"

#include "core/math/MathLib.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

namespace pio
{
	namespace sgf
	{
		Frustum::Frustum()
		{
			m_dirty.set();
		}

		Frustum::~Frustum() = default;

		void Frustum::calcPrj()
		{
			if (m_dirty.test(FM_BIT_PRJ))
			{
				m_dirty.reset(FM_BIT_PRJ);

				if (m_near <= 0.f || m_far <= 0.f)
				{
					LOG_ERR("err, invalid value, near[%.2f] and far[%.2f]", m_near, m_far);
					return;
				}

				if (m_fov <= 0.f || m_aspect <= 0.f)
				{
					LOG_ERR("err, invalid value, fov[%.2f], aspect[%.2f]", m_fov, m_aspect);
					return;
				}

				m_perspectMat = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
			}
		}

		void Frustum::calcOrtho()
		{
			if (m_dirty.test(FM_BIT_ORTHO))
			{
				m_dirty.reset(FM_BIT_ORTHO);
				m_orthoMat = glm::ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop);
			}
		}

		const glm::mat4 &Frustum::getPerspectMat()
		{
			calcPrj();
			return m_perspectMat;
		}

		const glm::mat4 &Frustum::getOrthoMat()
		{
			calcOrtho();
			return m_orthoMat;
		}

		void Frustum::setFov(float fov)
		{
			if (!MathLib::equalF(m_fov, fov))
			{
				m_fov = fov;
				m_dirty.set(FM_BIT_PRJ);
			}
		}

		void Frustum::setAspect(float aspect)
		{
			if (!MathLib::equalF(m_aspect, aspect))
			{
				m_aspect = aspect;
				m_dirty.set(FM_BIT_PRJ);
			}
		}

		void Frustum::setNearFar(float near, float far)
		{
			if (!MathLib::equalF(m_near, near) || !MathLib::equalF(m_far, far))
			{
				m_near = near;
				m_far = far;
				m_dirty.set(FM_BIT_PRJ);
			}
		}

		void Frustum::setOrtho(float l, float r, float b, float t)
		{
			if (!MathLib::equalF(m_orthoLeft, l) || !MathLib::equalF(m_orthoRight, r) ||
				!MathLib::equalF(m_orthoBottom, b) || !MathLib::equalF(m_orthoTop, t))
			{
				m_orthoLeft = l;
				m_orthoRight = r;
				m_orthoBottom = b;
				m_orthoTop = t;
				m_dirty.set(FM_BIT_ORTHO);
			}
		}
	}
}

