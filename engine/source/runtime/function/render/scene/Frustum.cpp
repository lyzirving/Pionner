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
		, m_change(true), m_stateStack()
		, m_perspectMat(1.f), m_perspectInvMat(1.f)
		, m_orthoLeft(0.f), m_orthoRight(1.f)
		, m_orthoBottom(0.f), m_orthoTop(1.f)
		, m_orthoMat(1.f)
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
			m_perspectMat = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
			m_perspectInvMat = glm::inverse(m_perspectMat);
			m_orthoMat = glm::ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop);
		}
	}

	const glm::mat4 &Frustum::getPerspectMat()
	{
		calcProjectMat();
		return m_perspectMat;
	}

	const glm::mat4 &Frustum::getPerspectInvMat()
	{
		calcProjectMat();
		return m_perspectInvMat;
	}

	const glm::mat4 &Frustum::getOrthoMat()
	{
		calcProjectMat();
		return m_orthoMat;
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

	void Frustum::setOrtho(float l, float r, float b, float t)
	{
		if (!MathLib::equalF(m_orthoLeft, l) || !MathLib::equalF(m_orthoRight, r) ||
			!MathLib::equalF(m_orthoBottom, b) || !MathLib::equalF(m_orthoTop, t))
		{
			m_orthoLeft = l;
			m_orthoRight = r;
			m_orthoBottom = b;
			m_orthoTop = t;
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
		FrustumState state = m_stateStack.back();
		m_fov = state.m_fov;
		m_aspect = state.m_aspect;
		m_near = state.m_near;
		m_far = state.m_far;
		m_stateStack.pop_back();
		m_change.store(true);
	}
}

