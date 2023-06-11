#include "ModelEntity.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/LightComp.h"

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
		m_type = ENTITY_TYPE_MODEL;
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
		std::shared_ptr<Entity> lightEntity{ nullptr };

		auto resource = param.resource;
		auto world = param.world;
		auto camera = param.sceneMgr->m_camera;
		auto frustum = param.sceneMgr->m_frustum;

		glm::mat4 modelMat = part->getTransform();
		glm::mat4 normalMat = MathLib::normalMat(modelMat);

		if (!(lightEntity = world->getEntity(World::ENTITY_POINT_LIGHT)) || !lightEntity->hasComp<LightComp>())
		{
			goto coloring_without_light;
		}

		LightComp &lightComp = lightEntity->getComp<LightComp>();

		if (!lightComp.m_light)
		{
			goto coloring_without_light;
		}

	coloring_with_light:

		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHTED_MESH, param.rhi)))
		{
			LOG_ERR("fail to get lighted-mesh shader");
			return false;
		}

		shader->use(true);

		lightComp.m_light->dealShader(shader);

		shader->setVec3("u_viewPos", camera->getCamPos());

		shader->setMat4("u_modelMat", modelMat);
		shader->setMat4("u_viewMat", camera->getViewMat());
		shader->setMat4("u_prjMat", frustum->getPerspectMat());
		shader->setMat4("u_normalMat", normalMat);

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

	coloring_without_light:

		return false;
	}
}