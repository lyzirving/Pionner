#include "Frustum.h"

namespace pio
{
	void Frustum::flush()
	{
		if (bDirty())
		{
			calcMat();
			invalidate(false);
		}
	}

	void Frustum::setAspect(float aspect)
	{
		if (!Math::Equal(m_aspect, aspect))
		{
			m_aspect = aspect;
			invalidate();
		}
	}

	void Frustum::setNear(float near)
	{
		if (!Math::Equal(m_near, near))
		{
			m_near = near;
			invalidate();
		}
	}

	void Frustum::setFar(float far)
	{
		if (!Math::Equal(m_far, far))
		{
			m_far = far;
			invalidate();
		}
	}
}