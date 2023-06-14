#include "DirectionLight.h"

#include "function/render/shader/Shader.h"

namespace Pionner
{
	DirectionLight::DirectionLight() : Light()
	{
		m_type = LIGHT_TYPE_DIRECTIONAL;
	}

	DirectionLight::DirectionLight(const glm::vec3 &direction)
		: Light()
	{
		m_type = LIGHT_TYPE_DIRECTIONAL;
		m_direction = direction;
	}

	DirectionLight::~DirectionLight()
	{
	}

	void DirectionLight::dealShader(const std::shared_ptr<Shader> &shader)
	{
		if (!shader || !shader->isInit())
			return;

		shader->setInt("u_light.type", m_type);
		shader->setVec3("u_light.direction", m_direction);

		shader->setVec3("u_light.ka", m_ka);
		shader->setVec3("u_light.kd", m_kd);
		shader->setVec3("u_light.ks", m_ks);

		shader->setFloat("u_light.ia", m_ia);
		shader->setFloat("u_light.id", m_id);
		shader->setFloat("u_light.is", m_is);

		shader->setFloat("u_light.shininess", m_shininess);
	}

	template<>
	bool Light::is<DirectionLight>() const
	{
		return m_type == LIGHT_TYPE_DIRECTIONAL;
	}
}