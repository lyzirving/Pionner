#include "PerspectiveFrustum.h"

namespace pio
{
	PerspectiveFrustum::PerspectiveFrustum(const PerspectiveFrustum &rhs) : Frustum(rhs), 
		m_fov(rhs.m_fov)
	{
	}

	PerspectiveFrustum &PerspectiveFrustum::operator=(const PerspectiveFrustum &rhs)
	{
		if(this != &rhs)
		{
			Frustum::operator=(rhs);
			m_fov = rhs.m_fov;
		}
		return *this;
	}

	void PerspectiveFrustum::setFov(float fov)
	{
		if (!Math::Equal(m_fov, fov))
		{
			m_fov = fov;
			invalidate();
		}
	}

	void PerspectiveFrustum::calcMat()
	{
		m_mat = Math::PerspectiveMat(m_fov, m_aspect, m_near, m_far);
	}
}