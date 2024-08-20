#include "OrthographicFrustum.h"

namespace pio
{
	OrthographicFrustum::OrthographicFrustum(const OrthographicFrustum& rhs) : Frustum(rhs),
		m_size(rhs.m_size)
	{
	}

	OrthographicFrustum& OrthographicFrustum::operator=(const OrthographicFrustum& rhs)
	{
		if (this != &rhs)
		{
			Frustum::operator=(rhs);
			m_size = rhs.m_size;
		}
		return *this;
	}

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
		m_mat = Frustum::OrthoMat(left(), right(), bottom(), top(), m_near, m_far);
	}
}