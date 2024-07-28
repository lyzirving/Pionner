#include "PerspectiveFrustum.h"

namespace pio
{
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