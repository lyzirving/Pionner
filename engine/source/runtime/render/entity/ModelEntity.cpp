#include "ModelEntity.h"

#include "render/RenderDef.h"

#include "render/rhi/Rhi.h"

#include "render/shader/ShaderMgr.h"

#include "render/scene/SceneMgr.h"
#include "render/scene/light/PointLight.h"

#include "render/resource/RenderResourceMgr.h"
#include "render/resource/buffer/GfxBuffer.h"
#include "render/resource/buffer/GfxFrameBuffer.h"

#include "core/log/LogSystem.h"
#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelEntity"

namespace pio
{
	ModelEntity::ModelEntity(RenderMode mode) : RenderEntity(mode)
	{
		m_type = RENDER_ENTITY_TYPE_MODEL;
	}

	ModelEntity::~ModelEntity() = default;

	bool ModelEntity::dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								 /*out*/std::shared_ptr<Shader> &shader)
	{
		switch (m_renderMode)
		{
			case pio::RENDER_MODE_WIRE_FRAME:
				break;
			case pio::RENDER_MODE_SOLID:
				break;
			case pio::RENDER_MODE_MATERIAL_DISPLAY:
				return coloringMaterialDisplay(param, part, shader);
			case pio::RENDER_MODE_RENDERED_DISPLAY:
				return coloringRenderDisplay(param, part, shader);
			default:
				break;
		}
		LOG_ERR("invalid render mode[%u]", m_renderMode);
		return false;
	}

	bool ModelEntity::dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		LightType curLight = param.sceneMgr->m_curLight;
		if (curLight == LIGHT_TYPE_DIRECTIONAL)
		{
			return dealDirectionLightDepthShader(param, part, shader);
		}
		else if (curLight == LIGHT_TYPE_POINT)
		{
			return dealPointLightDepthShader(param, part, shader);
		}
		LOG_ERR("invalid light type[%u]", curLight);
		return false;
	}

	bool ModelEntity::coloringRenderDisplay(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto sceneMgr = param.sceneMgr;
		auto light = sceneMgr->m_lights[sceneMgr->m_curLight];

		bool lightExist = (light != nullptr);
		bool normalExist = part->m_material.normValid();

		if (lightExist && normalExist)
		{
			return coloringWithNormAndLight(param, part, shader);
		}
		else if (lightExist)
		{
			return coloringWithLight(param, part, shader);
		}
		else if (!lightExist)
		{
			return coloringMaterialDisplay(param, part, shader);
		}
	}

	bool ModelEntity::coloringWithLight(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto resource = param.resource;
		auto sceneMgr = param.sceneMgr;

		auto camera = sceneMgr->m_camera;
		auto frustum = sceneMgr->m_frustum;

		auto light = sceneMgr->m_lights[sceneMgr->m_curLight];

		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHTED_MESH)))
		{
			LOG_ERR("fail to get lighted_nor_mesh shader");
			return false;
		}

		int32_t texUnit{ 0 };
		std::shared_ptr<GfxBuffer> texture{ nullptr };
		glm::mat4 modelMat = part->getTransform();

		shader->use(true);

		//>>>>>>> upload material >>>>>>>
		if (part->m_material.ambientValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_ambientSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.ambTexture", texUnit++);
			shader->setInt("u_material.hasAmbTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasAmbTex", 0);
		}

		if (part->m_material.diffValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_diffSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.diffuseTexture", texUnit++);
			shader->setInt("u_material.hasDiffTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasDiffTex", 0);
		}

		if (part->m_material.specValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_specSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.specTexture", texUnit++);
			shader->setInt("u_material.hasSpecTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasSpecTex", 0);
		}

		shader->setInt("u_material.colorMode", part->m_material.m_mode);

		shader->setVec3("u_material.ka", part->m_material.m_colorAmbient);
		shader->setVec3("u_material.kd", part->m_material.m_colorDiffuse);
		shader->setVec3("u_material.ks", part->m_material.m_colorSpecular);
		//>>>>>>> finish uploading material >>>>>>>

		light->dealShader(param, shader, (texUnit != 0) ? (texUnit++) : 1);

		shader->setMat4("u_modelMat", modelMat);
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());

		shader->setMat3("u_normalMat", MathLib::normalMat(modelMat));

		return true;
	}

	bool ModelEntity::coloringWithNormAndLight(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto resource = param.resource;
		auto sceneMgr = param.sceneMgr;

		auto camera = sceneMgr->m_camera;
		auto frustum = sceneMgr->m_frustum;

		auto light = sceneMgr->m_lights[sceneMgr->m_curLight];

		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHTED_NORM_MESH)))
		{
			LOG_ERR("fail to get lighted_nor_mesh shader");
			return false;
		}

		int32_t texUnit{ 0 };
		std::shared_ptr<GfxBuffer> texture{ nullptr };
		glm::mat4 modelMat = part->getTransform();

		shader->use(true);

		if (!(texture = resource->find(BUF_TEXTURE, part->m_material.m_normalSlot)))
		{
			LOG_ERR("err, normal texture is invalid!");
			shader->use(false);
			return false;
		}

		//>>>>>>> upload material >>>>>>>
		texture->upload();
		texture->bindTarget(texUnit);
		shader->setInt("u_material.normTexture", texUnit++);
		shader->setInt("u_material.hasNormal", 1);

		if (part->m_material.ambientValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_ambientSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.ambTexture", texUnit++);
			shader->setInt("u_material.hasAmbTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasAmbTex", 0);
		}

		if (part->m_material.diffValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_diffSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.diffuseTexture", texUnit++);
			shader->setInt("u_material.hasDiffTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasDiffTex", 0);
		}

		if (part->m_material.specValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_specSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.specTexture", texUnit++);
			shader->setInt("u_material.hasSpecTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasSpecTex", 0);
		}

		shader->setInt("u_material.colorMode", part->m_material.m_mode);

		shader->setVec3("u_material.ka", part->m_material.m_colorAmbient);
		shader->setVec3("u_material.kd", part->m_material.m_colorDiffuse);
		shader->setVec3("u_material.ks", part->m_material.m_colorSpecular);
		//>>>>>>> finish uploading material >>>>>>>
		light->dealShader(param, shader, (texUnit != 0) ? (texUnit++) : 1);
			
		shader->setMat4("u_modelMat", modelMat);
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());

		shader->setMat3("u_normalMat", MathLib::normalMat(modelMat));

		return true;
	}

	bool ModelEntity::coloringMaterialDisplay(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto resource = param.resource;
		auto sceneMgr = param.sceneMgr;

		auto camera = sceneMgr->m_camera;
		auto frustum = sceneMgr->m_frustum;

		if (!(shader = param.shaderMgr->get(SHADER_TYPE_MESH)))
		{
			LOG_ERR("fail to get lighted_nor_mesh shader");
			return false;
		}

		shader->use(true);

		int32_t texUnit{ 0 };
		std::shared_ptr<GfxBuffer> texture{ nullptr };

		if (part->m_material.diffValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_diffSlot)))
		{
			texture->upload();
			texture->bindTarget(texUnit);
			shader->setInt("u_material.diffuseTexture", texUnit++);
			shader->setInt("u_material.hasDiffTex", 1);
		}
		else
		{
			shader->setInt("u_material.hasDiffTex", 0);
		}

		shader->setInt("u_material.colorMode", part->m_material.m_mode);

		shader->setVec3("u_material.ka", part->m_material.m_colorAmbient);
		shader->setVec3("u_material.kd", part->m_material.m_colorDiffuse);
		shader->setVec3("u_material.ks", part->m_material.m_colorSpecular);

		shader->setMat4("u_modelMat", part->getTransform());
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());

		return true;
	}

	bool ModelEntity::dealDirectionLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto scene = param.sceneMgr;
		LightType curLight = scene->m_curLight;

		auto light = scene->m_lights[curLight];
		if (!light)
		{
			LOG_ERR("current light is invalid");
			return false;
		}

		shader = param.shaderMgr->get(SHADER_TYPE_SHADOW_MAP);

		if (!shader || !shader->isInit())
		{
			LOG_ERR("shadow map shader is invalid");
			return false;
		}

		shader->use(true);
		shader->setMat4("u_modelMat", part->getTransform());
		shader->setMat4("u_lightViewMat", light->getViewMat());
		shader->setMat4("u_lightPrjMat", light->getPrjMat());

		return true;
	}

	bool ModelEntity::dealPointLightDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		auto scene = param.sceneMgr;
		LightType curLight = scene->m_curLight;

		auto light = scene->m_lights[curLight];
		if (!light)
		{
			LOG_ERR("current light is invalid");
			return false;
		}
		auto pPtLight = light->to<PointLight>();
		if (!pPtLight)
		{
			LOG_ERR("fail to convert to PointLight ptr");
			return false;
		}

		shader = param.shaderMgr->get(SHADER_TYPE_POINT_SHADOW_MAP);

		if (!shader || !shader->isInit())
		{
			LOG_ERR("pt shadow map shader is invalid");
			return false;
		}

		shader->use(true);

		shader->setMat4("u_modelMat", part->getTransform());
		shader->setVec3("u_lightPos", light->position());
		shader->setFloat("u_farPlane", light->far());
		for (uint32_t i = 0; i < 6; ++i)
		{
			LightDir dir = LightDir(i);
			shader->setMat4("u_lightMats[" + std::to_string(i) + "]", pPtLight->getLightMat(dir));
		}
		return true;
	}
}