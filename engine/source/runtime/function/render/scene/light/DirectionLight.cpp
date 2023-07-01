#include <glm/gtc/matrix_transform.hpp>

#include "DirectionLight.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxFrameBuffer.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/shader/Shader.h"
#include "function/render/RenderDef.h"

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

	void DirectionLight::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader, uint32_t texUnit)
	{
		if (!shader || !shader->isInit())
			return;

		auto resource = param.resource;
		auto scene = param.sceneMgr;
		auto light = scene->m_lights[scene->m_curLight];

		// filed define as an uniform
		shader->setVec3("u_viewPos", scene->m_camera->getCamPos());

		shader->setVec3("u_lightPos", m_position);
		shader->setVec3("u_lightDir", m_direction);
		shader->setMat4("u_lightViewMat", getViewMat());
		shader->setMat4("u_lightPrjMat", getPrjMat());

		// filed defined as a component of a uniform struct
		shader->setInt("u_light.type", m_type);

		shader->setVec3("u_light.ka", m_ka);
		shader->setVec3("u_light.kd", m_kd);
		shader->setVec3("u_light.ks", m_ks);

		shader->setFloat("u_light.ia", m_ia);
		shader->setFloat("u_light.id", m_id);
		shader->setFloat("u_light.is", m_is);

		shader->setFloat("u_light.shininess", m_shininess);

		auto shadowBuf = resource->createHolderBuffer(BUF_TEXTURE);
		shadowBuf->setHolderId(light->getDepthFbo()->getAttachment(DEPTH_ATTACH));
		shadowBuf->bindTarget(texUnit);
		shader->setInt("u_depthTexture", texUnit);
	}

	void DirectionLight::calcMatrix()
	{
		m_viewMat = glm::lookAt(m_position, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
		m_prjMat = glm::ortho(-10.f, 10.f, -10.f, 10.f, m_near, m_far);
	}

	template<>
	bool Light::is<DirectionLight>() const
	{
		return m_type == LIGHT_TYPE_DIRECTIONAL;
	}

	template<>
	DirectionLight *Light::to<DirectionLight>() const
	{
		if (is<DirectionLight>())
		{
			return (DirectionLight *)(this);
		}
		return nullptr;
	}
}