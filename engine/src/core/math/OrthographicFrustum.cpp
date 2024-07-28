#include "OrthographicFrustum.h"

namespace pio
{
	void OrthographicFrustum::setSize(float size)
	{
		if (!Math::Equal(m_size, size))
		{
			m_size = size;
			invalidate();
		}
	}

	void OrthographicFrustum::calcMat()
	{
		m_mat = Math::OrthoMat(left(), right(), bottom(), top(), m_near, m_far);
	}
}