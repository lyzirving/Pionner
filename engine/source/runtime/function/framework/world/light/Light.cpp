#include "function/framework/world/light/Light.h"

namespace Pionner
{
	Light::Light() : m_ka(1.f), m_kd(1.f), m_ks(1.f)
		, m_ia(0.2f), m_id(0.6f), m_is(0.7f)
		, m_shininess(64.f)
		, m_type(LIGHT_TYPE_NONE)
	{
	}

	Light::~Light()
	{
	}

	const glm::vec3 &Light::ambient()
	{
		return m_ka;
	}

	const glm::vec3 &Light::diffuse()
	{
		return m_kd;
	}

	const glm::vec3 &Light::specular()
	{
		return m_ks;
	}

	float Light::ambientIntensity()
	{
		return m_ia;
	}

	float Light::diffuseIntensity()
	{
		return m_id;
	}

	float Light::specularIntensity()
	{
		return m_is;
	}

	float Light::shininess()
	{
		return m_shininess;
	}

	LightType Light::type()
	{
		return m_type;
	}
}