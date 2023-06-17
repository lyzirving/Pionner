#include "ModelEntity.h"

#include "function/render/RenderDef.h"

#include "function/render/rhi/Rhi.h"

#include "function/render/shader/ShaderMgr.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "core/log/LogSystem.h"
#include "core/math/MathLib.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ModelEntity"

namespace Pionner
{
	ModelEntity::ModelEntity() : RenderEntity()
	{
		m_type = RENDER_ENTITY_TYPE_MODEL;
	}

	ModelEntity::~ModelEntity() = default;

	std::shared_ptr<RenderEntity> ModelEntity::makeEmptyEntity()
	{
		return std::shared_ptr<RenderEntity>(new ModelEntity);
	}

	bool ModelEntity::dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								 /*out*/std::shared_ptr<Shader> &shader,
								 /*out*/std::shared_ptr<GfxBuffer> &texture)
	{
		auto resource = param.resource;
		auto sceneMgr = param.sceneMgr;

		auto camera = sceneMgr->m_camera;
		auto frustum = sceneMgr->m_frustum;
		auto light = sceneMgr->m_lights[sceneMgr->m_curLight];

		glm::mat4 modelMat = part->getTransform();
		bool lightExist = (light != nullptr);

		ShaderType shaderType = lightExist ? SHADER_TYPE_LIGHTED_MESH : SHADER_TYPE_MESH;

		if (!(shader = param.shaderMgr->get(shaderType, param.rhi)))
		{
			LOG_ERR("fail to get lighted-mesh shader");
			return false;
		}

		shader->use(true);

		if (lightExist)
		{
			light->dealShader(shader);
			shader->setVec3("u_viewPos", camera->getCamPos());
			shader->setMat4("u_normalMat", MathLib::normalMat(modelMat));
		}

		shader->setMat4("u_modelMat", modelMat);
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());

		shader->setInt("u_material.colorMode", part->m_material.m_mode);
		shader->setInt("u_material.texType", part->m_material.m_type);

		if (part->m_material.slotValid() && (texture = resource->find(BUF_TEXTURE, part->m_material.m_slot)))
		{
			texture->upload();
			texture->bindTarget(part->m_partIndex);

			std::string sampler = (part->m_material.m_type == MAT_DIFFUSE) ? "u_material.diffuseTexture" : "u_material.specTexture";
			shader->setInt(sampler, part->m_partIndex);
			shader->setInt("u_material.hasTexture", 1);
		}
		else
		{
			shader->setInt("u_material.hasTexture", 0);
		}

		shader->setVec3("u_material.ka", part->m_material.m_colorAmbient);
		shader->setVec3("u_material.kd", part->m_material.m_colorDiffuse);
		shader->setVec3("u_material.ks", part->m_material.m_colorSpecular);

		return true;
	}
}