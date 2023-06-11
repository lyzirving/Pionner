#include "LightEntity.h"

#include "function/render/RenderDef.h"

#include "function/render/rhi/Rhi.h"

#include "function/render/shader/ShaderMgr.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

namespace Pionner
{
	LightEntity::LightEntity()
	{
		m_type = ENTITY_TYPE_LIGHT;
	}

	LightEntity::~LightEntity() = default;

	bool LightEntity::dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								 /*out*/std::shared_ptr<Shader> &shader,
								 /*out*/std::shared_ptr<GfxBuffer> &texture)
	{
		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHT_CASTER, param.rhi)))
		{
			return false;
		}

		auto resource = param.resource;
		auto camera = param.sceneMgr->m_camera;
		auto frustum = param.sceneMgr->m_frustum;

		shader->use(true);

		shader->setMat4("u_modelMat", part->getTransform());
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());

		shader->setInt("u_material.shadingModel", part->m_material.m_mode);
		shader->setInt("u_material.texType", part->m_material.m_type);

		if (part->m_material.slotValid() && (texture = resource->find(DATA_TEXTURE, part->m_material.m_slot)))
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

	std::shared_ptr<RenderEntity> LightEntity::makeEmptyEntity()
	{
		return std::shared_ptr<RenderEntity>(new LightEntity);
	}
}