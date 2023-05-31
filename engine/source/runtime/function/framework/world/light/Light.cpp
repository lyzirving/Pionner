#include "function/framework/world/light/Light.h"

#include "function/framework/world/light/PointLight.h"

namespace Pionner
{
	Light::Light()
		: m_position(0.f), m_direction(0.f)
		, m_ka(1.f), m_kd(1.f), m_ks(1.f)
		, m_ia(0.2f), m_id(0.6f), m_is(0.7f)
		, m_shininess(64.f)
		, m_type(LIGHT_TYPE_NONE)
	{
	}

	Light::~Light()
	{
	}

	std::shared_ptr<Light> Light::createLight(LightType type)
	{
		switch (type)
		{
			case Pionner::POINT_LIGHT:
				return std::shared_ptr<Light>(new PointLight);
			default:
				return std::shared_ptr<Light>();
		}
	}
}