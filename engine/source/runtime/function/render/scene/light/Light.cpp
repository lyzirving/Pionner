#include "Light.h"

#include "PointLight.h"
#include "DirectionLight.h"

namespace Pionner
{
	Light::Light()
		: m_position(0.f), m_direction(0.f)
		, m_ka(1.f), m_kd(1.f), m_ks(1.f)
		, m_ia(0.2f), m_id(0.6f), m_is(0.7f)
		, m_shininess(64.f)
		, m_type(LIGHT_TYPE_DIRECTIONAL)
	{
	}

	Light::~Light()
	{
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
}