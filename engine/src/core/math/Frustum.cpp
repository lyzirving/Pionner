#include "Frustum.h"

namespace pio
{
	Frustum::Frustum(const Frustum &rhs) : 
		m_type(rhs.m_type), m_aspect(rhs.m_aspect), 
		m_near(rhs.m_near), m_far(rhs.m_far), 
		m_mat(rhs.m_mat), m_invalidate(rhs.m_invalidate)
	{
	}

	Frustum &Frustum::operator=(const Frustum &rhs)
	{
		if(this != &rhs)
		{
			m_type = rhs.m_type;
			m_aspect = rhs.m_aspect;
			m_near = rhs.m_near;
			m_far = rhs.m_far;
			m_mat = rhs.m_mat;
			m_invalidate = rhs.m_invalidate;
		}
		return *this;
	}

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