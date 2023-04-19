#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "function/render/scene/Frustum.h"

#include "core/log/LogSystem.h"
#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

namespace Pionner
{
	Frustum::Frustum(float fov, float aspect, float near, float far)
		: m_fov(fov), m_aspect(aspect), m_near(near), m_far(far)
		, m_change(true), m_projectMat(1.f), m_stateStack()
	{
	}

	Frustum::~Frustum() = default;

	void Frustum::calcProjectMat()
	{
		if (m_change.load())
		{
			m_change.store(false);
			if (m_near <= 0.f || m_far <= 0.f)
			{
				LOG_ERR("err, invalid value, near[%.2f] and far[%.2f]", m_near, m_far);
				return;
			}
			m_projectMat = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
		}
	}

	const glm::mat4 &Frustum::getProjectMat()
	{
		calcProjectMat();
		return m_projectMat;
	}

	void Frustum::setFov(float fov)
	{
		if (!MathLib::equalF(m_fov, fov))
		{
			m_fov = fov;
			m_change.store(true);
		}
	}

	void Frustum::setAspect(float aspect)
	{
		if (!MathLib::equalF(m_aspect, aspect))
		{
			m_aspect = aspect;
			m_change.store(true);
		}
	}

	void Frustum::setNearFar(float near, float far)
	{
		if (!MathLib::equalF(m_near, near) || !MathLib::equalF(m_far, far))
		{
			m_near = near;
			m_far = far;
			m_change.store(true);
		}
	}

	void Frustum::restoreState()
	{
		FrustumState state{ m_fov, m_aspect, m_near, m_far };
		m_stateStack.push_back(state);
	}

	void Frustum::popState()
	{
		if (m_stateStack.empty())
		{
			LOG_ERR("state stack is empty, err operation");
			return;
		}
		FrustumState state = m_stateStack.front();
		m_fov = state.m_fov;
		m_aspect = state.m_aspect;
		m_near = state.m_near;
		m_far = state.m_far;
		m_stateStack.pop_front();
		m_change.store(true);
	}
}
