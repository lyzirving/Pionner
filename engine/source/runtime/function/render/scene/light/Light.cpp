#include "Light.h"

#include "PointLight.h"
#include "DirectionLight.h"

#include "function/render/RenderDef.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxFrameBuffer.h"

#include "core/math/MathLib.h"

namespace Pionner
{
	Light::Light()
		: m_position(0.f), m_direction(0.f)
		, m_ka(1.f), m_kd(1.f), m_ks(1.f)
		, m_ia(0.2f), m_id(0.6f), m_is(0.7f)
		, m_shininess(64.f), m_near(0.1f), m_far(30.f)
		, m_viewMat(1.f), m_prjMat(1.f), m_dataChange(true)
		, m_type(LIGHT_TYPE_DIRECTIONAL)
		, m_depthFbo(nullptr)
	{
	}

	Light::~Light()
	{
		m_depthFbo.reset();
	}

	const glm::mat4 &Light::getViewMat()
	{
		if (m_dataChange.load())
		{
			calcMatrix();
			m_dataChange.store(false);
		}
		return m_viewMat;
	}

	const glm::mat4 &Light::getPrjMat()
	{
		if (m_dataChange.load())
		{
			calcMatrix();
			m_dataChange.store(false);
		}
		return m_prjMat;
	}

	std::shared_ptr<Light> Light::createLight(LightType type)
	{
		switch (type)
		{
			case LIGHT_TYPE_DIRECTIONAL:
				return std::shared_ptr<Light>(new DirectionLight);
			case LIGHT_TYPE_POINT:
				return std::shared_ptr<Light>(new PointLight);
			default:
				return std::shared_ptr<Light>();
		}
	}

	void Light::setPosition(const glm::vec3 &pos)
	{
		if (!MathLib::equalVec3(pos, m_position))
		{
			m_position = pos;
			m_dataChange.store(true);
		}
	}

	void Light::setDirection(const glm::vec3 &dir)
	{
		if (!MathLib::equalVec3(dir, m_direction))
		{
			m_direction = dir;
			m_dataChange.store(true);
		}
	}

	void Light::setNear(float near)
	{
		if (!MathLib::equalF(m_near, near))
		{
			m_near = near;
			m_dataChange.store(true);
		}
	}

	void Light::setFar(float far)
	{
		if (!MathLib::equalF(m_far, far))
		{
			m_far = far;
			m_dataChange.store(true);
		}
	}

	void Light::setColor(const glm::vec3 &ka, const glm::vec3 &kd, const glm::vec3 &ks)
	{
		m_ka = ka;
		m_kd = kd;
		m_ks = ks;
	}

	void Light::setIntensity(float ia, float id, float is)
	{
		m_ia = ia;
		m_id = id;
		m_is = is;
	}

	void Light::initDepthBuffer(uint32_t width, uint32_t height, const RenderParam &param)
	{
		if (!m_depthFbo)
		{
			auto resource = param.resource;
			m_depthFbo = resource->allocFbo(BUF_DEPTH_FRAMEBUFFER);
			m_depthFbo->setSize(width, height);
		}
	}
}