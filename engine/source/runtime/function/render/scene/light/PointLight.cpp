#include "PointLight.h"
#include "function/render/shader/Shader.h"

namespace Pionner
{
	PointLight::PointLight() : Light()
		, m_attenParamConst(1.f), m_attenParamLinear(0.045f), m_attenParamQuad(0.0075f)
	{
		m_type = LIGHT_TYPE_POINT;
	}

	PointLight::PointLight(const glm::vec3 &position) : Light()
		, m_attenParamConst(1.f), m_attenParamLinear(0.045f), m_attenParamQuad(0.0075f)
	{
		m_type = LIGHT_TYPE_POINT;
		m_position = position;
	}

	PointLight::~PointLight()
	{
	}

	void PointLight::dealShader(const std::shared_ptr<Shader> &shader)
	{
		if (!shader || !shader->isInit())
			return;

		shader->setInt("u_light.type", m_type);
		shader->setVec3("u_light.pos", m_position);

		shader->setVec3("u_light.ka", m_ka);
		shader->setVec3("u_light.kd", m_kd);
		shader->setVec3("u_light.ks", m_ks);

		shader->setFloat("u_light.ia", m_ia);
		shader->setFloat("u_light.id", m_id);
		shader->setFloat("u_light.is", m_is);

		shader->setFloat("u_light.shininess", m_shininess);

		shader->setFloat("u_light.attParamConst", m_attenParamConst);
		shader->setFloat("u_light.attParamLinear", m_attenParamLinear);
		shader->setFloat("u_light.attParamQuad", m_attenParamQuad);
	}

	void PointLight::setAttenuation(float constVal, float linearVal, float quadVal)
	{
		m_attenParamConst = constVal;
		m_attenParamLinear = linearVal;
		m_attenParamQuad = quadVal;
	}

	template<>
	bool Light::is<PointLight>() const
	{
		return m_type == LIGHT_TYPE_POINT;
	}
}