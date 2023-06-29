#include <glm/gtc/matrix_transform.hpp>

#include "PointLight.h"

#include "function/render/resource/buffer/GfxFrameBuffer.h"
#include "function/render/shader/Shader.h"

namespace Pionner
{
	PointLight::PointLight() : Light()
		, m_attenParamConst(1.f), m_attenParamLinear(0.045f), m_attenParamQuad(0.0075f)
		, m_lightMat()
	{
		m_type = LIGHT_TYPE_POINT;
	}

	PointLight::PointLight(const glm::vec3 &position) : Light()
		, m_attenParamConst(1.f), m_attenParamLinear(0.045f), m_attenParamQuad(0.0075f)
		, m_lightMat()
	{
		m_type = LIGHT_TYPE_POINT;
		m_position = position;
	}

	PointLight::~PointLight() = default;

	void PointLight::dealShader(const std::shared_ptr<Shader> &shader)
	{
		if (!shader || !shader->isInit())
			return;

		shader->setVec3("u_lightPos", m_position);

		shader->setInt("u_light.type", m_type);

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

	const glm::mat4 &PointLight::getLightMat(LightDir dir)
	{
		if (m_dataChange.load())
		{
			calcMatrix();
			m_dataChange.store(false);
		}
		return m_lightMat[dir];
	}

	void PointLight::calcMatrix()
	{
		glm::ivec2 size = glm::ivec2{ m_depthFbo->getWidth(), m_depthFbo->getHeight() };
		m_prjMat = glm::perspective(glm::radians(90.0f), (float)size.x / (float)size.y, m_near, m_far);

		m_lightMat[LIGHT_DIR_POSITIVE_X] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
		m_lightMat[LIGHT_DIR_NEGATIVE_X] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
		m_lightMat[LIGHT_DIR_POSITIVE_Y] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
		m_lightMat[LIGHT_DIR_NEGATIVE_Y] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -1.f));
		m_lightMat[LIGHT_DIR_NEGATIVE_Z] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, -1.f, 0.f));
		m_lightMat[LIGHT_DIR_POSITIVE_Z] = m_prjMat * glm::lookAt(m_position, m_position + glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, -1.f, 0.f));
	}

	template<>
	bool Light::is<PointLight>() const
	{
		return m_type == LIGHT_TYPE_POINT;
	}

	template<>
	PointLight *Light::to<PointLight>() const
	{
		if (is<PointLight>())
		{
			return (PointLight *)(this);
		}
		return nullptr;
	}
}