#include "Frustum.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

namespace pio
{
	Frustum::Frustum()
	{
		m_dirty.set();
	}

	Frustum::Frustum(const Frustum &rhs) : m_fov(rhs.m_fov), m_aspect(rhs.m_aspect), m_near(rhs.m_near), m_far(rhs.m_far)
		, m_orthoLeft(rhs.m_orthoLeft), m_orthoRight(rhs.m_orthoRight), m_orthoBottom(rhs.m_orthoBottom), m_orthoTop(rhs.m_orthoTop)
		, m_perspectMat(rhs.m_perspectMat), m_orthoMat(rhs.m_orthoMat)
	{
		m_dirty.set();
	}

	Frustum &Frustum::operator=(const Frustum &rhs)
	{
		if (this != &rhs)
		{
			m_fov = rhs.m_fov;
			m_aspect = rhs.m_aspect;
			m_near = rhs.m_near;
			m_far = rhs.m_far;
			m_orthoLeft = rhs.m_orthoLeft;
			m_orthoRight = rhs.m_orthoRight;
			m_orthoBottom = rhs.m_orthoBottom;
			m_orthoTop = rhs.m_orthoTop;
			m_perspectMat = rhs.m_perspectMat;
			m_orthoMat = rhs.m_orthoMat;
			m_dirty.set();
		}
		return *this;
	}

	void Frustum::calcPrj()
	{
		if (m_dirty.test(FM_BIT_PRJ))
		{
			m_dirty.reset(FM_BIT_PRJ);

			if (m_near <= 0.f || m_far <= 0.f)
			{
				LOGE("err, invalid value, near[%.2f] and far[%.2f]", m_near, m_far);
				return;
			}

			if (m_fov <= 0.f || m_aspect <= 0.f)
			{
				LOGE("err, invalid value, fov[%.2f], aspect[%.2f]", m_fov, m_aspect);
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
			m_orthoMat = glm::ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop, m_near, m_far);
		}
	}

	void Frustum::setFov(float fov)
	{
		if (!Math::Equal(m_fov, fov))
		{
			m_fov = fov;
			m_dirty.set(FM_BIT_PRJ);
		}
	}

	void Frustum::setAspect(float aspect)
	{
		if (!Math::Equal(m_aspect, aspect))
		{
			m_aspect = aspect;
			m_dirty.set(FM_BIT_PRJ);
		}
	}

	void Frustum::setNearFar(float near, float far)
	{
		if (!Math::Equal(m_near, near) || !Math::Equal(m_far, far))
		{
			m_near = near;
			m_far = far;
			m_dirty.set(FM_BIT_PRJ);
		}
	}

	void Frustum::setOrtho(float l, float r, float b, float t)
	{
		if (!Math::Equal(m_orthoLeft, l) || !Math::Equal(m_orthoRight, r) ||
			!Math::Equal(m_orthoBottom, b) || !Math::Equal(m_orthoTop, t))
		{
			m_orthoLeft = l;
			m_orthoRight = r;
			m_orthoBottom = b;
			m_orthoTop = t;
			m_dirty.set(FM_BIT_ORTHO);
		}
	}
}

