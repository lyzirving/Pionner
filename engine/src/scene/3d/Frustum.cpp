#include "Frustum.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

namespace pio
{
	Frustum::Frustum(const Frustum& rhs) :
		m_type(rhs.m_type), m_aspect(rhs.m_aspect),
		m_near(rhs.m_near), m_far(rhs.m_far),
		m_mat(rhs.m_mat), m_invalidate(rhs.m_invalidate)
	{
	}

	Frustum& Frustum::operator=(const Frustum& rhs)
	{
		if (this != &rhs)
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

	glm::mat4 Frustum::PerspectiveMat(float fov, float aspect, float near, float far)
	{
		if (Math::LessEqual(fov, 0.f) || Math::LessEqual(aspect, 0.f))
		{
			LOGE("err! invalid input, fov[%f], aspect[%f]", fov, aspect);
			return glm::mat4(1.f);
		}

		if (Math::LessEqual(near, 0.f) || Math::LessEqual(far, 0.f))
		{
			LOGE("err! invalid input, near[%f] and far[%f]", near, far);
			return glm::mat4(1.f);
		}

		return glm::perspective(glm::radians(fov), aspect, near, far);
	}

	glm::mat4 Frustum::OrthoMat(float left, float right, float bottom, float top, float near, float far)
	{
		return glm::ortho(left, right, bottom, top, near, far);
	}
}