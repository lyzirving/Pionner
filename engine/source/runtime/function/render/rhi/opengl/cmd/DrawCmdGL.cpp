#include <glm/gtc/matrix_transform.hpp>

#include "function/render/rhi/opengl/cmd/DrawCmdGL.h"
#include "function/render/rhi/RhiHeader.h"
#include "function/render/rhi/opengl/GLHelper.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/shader/ShaderMgr.h"
#include "function/render/shader/Shader.h"

#include "function/render/entity/RenderEntity.h"
#include "function/render/RenderDef.h"

#include "function/framework/comp/MeshComp.h"
#include "function/framework/comp/TransformComp.h"
#include "function/framework/comp/LightComp.h"

#include "function/framework/world/World.h"

#include "function/render/geo/Geometry.h"

#include "core/math/MathLib.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DrawCmdGL"

namespace Pionner
{
	DrawCmdGL::DrawCmdGL()
	{
	}

	DrawCmdGL::~DrawCmdGL()
	{
	}

	void DrawCmdGL::drawEntity(std::shared_ptr<RenderEntity> &entity, RenderParam &param)
	{
		for (auto &part : entity->m_parts)
		{
			drawPart(part, param);
		}

		if (!entity->m_children.empty())
		{
			for (auto &child : entity->m_children)
			{
				drawEntity(child, param);
			}
		}
	}

	void DrawCmdGL::drawCircle(std::shared_ptr<MeshComp> &mesh, RenderParam &param)
	{
		if (!mesh || !mesh->m_initialized || mesh->m_vBufSlot < 0 || mesh->m_indBufSlot < 0)
		{
			return;
		}
		auto resource = param.resource;
		auto vertexBuf = resource->find(DATA_VERTEX, mesh->m_vBufSlot);
		auto indiceBuf = resource->find(DATA_INDICE, mesh->m_indBufSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		auto shader = param.shaderMgr->get(SHADER_TYPE_CIRCLE, param.rhi);

		if (!shader)
		{
			LOG_ERR("shader is invalid");
			return;
		}

		shader->use(true);

		std::string key{ "circle_radius" };
		auto itr = mesh->m_args.find(key);
		if (itr != mesh->m_args.end())
		{
			shader->setFloat("u_radius", itr->second.argFlt);
		}

		key = "viewport_size";
		itr = mesh->m_args.find(key);
		if (itr != mesh->m_args.end())
		{
			shader->setVec2("u_viewportSize", itr->second.argVec2);
		}

		shader->setVec4("u_color", mesh->m_color);

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when drawing circle");

		vertexBuf->unbind();
		indiceBuf->unbind();

		shader->use(false);
	}

	void DrawCmdGL::drawInfiniteGrid(std::shared_ptr<MeshComp> &mesh, RenderParam &param)
	{
		if (!mesh || !mesh->m_initialized || mesh->m_vBufSlot < 0 || mesh->m_indBufSlot < 0)
		{
			return;
		}
		auto resource = param.resource;
		auto vertexBuf = resource->find(DATA_VERTEX, mesh->m_vBufSlot);
		auto indiceBuf = resource->find(DATA_INDICE, mesh->m_indBufSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		auto shader = param.shaderMgr->get(SHADER_TYPE_INFINITE_GRID, param.rhi);

		if (!shader)
		{
			LOG_ERR("shader is invalid");
			return;
		}

		shader->use(true);

		shader->setVec4("u_lineColor", mesh->m_color);
		shader->setMat4("u_modelMat", mesh->m_mat);
		shader->setMat4("u_viewMat", param.sceneMgr->m_camera->getViewMat());
		shader->setMat4("u_prjMat", param.sceneMgr->m_frustum->getPerspectMat());

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when drawing grid");

		vertexBuf->unbind();
		indiceBuf->unbind();

		shader->use(false);
	}

	void DrawCmdGL::drawColorGeometry(Geometry &geometry, RenderParam &param)
	{
		auto meshComp = geometry.getMeshComp();
		if (!meshComp || !meshComp->m_initialized || meshComp->m_vBufSlot < 0 || meshComp->m_indBufSlot < 0)
		{
			LOG_ERR("mesh comp is invalid");
			return;
		}

		auto transComp = geometry.getTransformComp();
		if (!transComp)
		{
			LOG_ERR("trans comp is invalid");
			return;
		}

		auto resource = param.resource;
		auto vertexBuf = resource->find(DATA_VERTEX, meshComp->m_vBufSlot);
		auto indiceBuf = resource->find(DATA_INDICE, meshComp->m_indBufSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		auto shader = param.shaderMgr->get(SHADER_TYPE_COLOR_GEOMETRY, param.rhi);

		if (!shader)
		{
			LOG_ERR("shader is invalid");
			return;
		}

		shader->use(true);

		shader->setVec4("u_color", meshComp->m_color);
		shader->setMat4("u_modelMat", transComp->getMat());
		shader->setMat4("u_viewMat", param.sceneMgr->m_camera->getViewMat());
		shader->setMat4("u_prjMat", param.sceneMgr->m_frustum->getPerspectMat());

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when color geometry");

		vertexBuf->unbind();
		indiceBuf->unbind();

		shader->use(false);
	}

	void DrawCmdGL::drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}
		std::shared_ptr<GfxBuffer>    texture{ nullptr };
		std::shared_ptr<Entity>       lightEntity{ nullptr };
		std::shared_ptr<RenderEntity> lightRenderEntity{ nullptr };
		std::shared_ptr<Shader>       shader{ nullptr };

		glm::mat4 modelMat{ 1.f }, normalMat{ 1.f };

		std::shared_ptr<RenderResourceMgr> resource = param.resource;
		std::shared_ptr<Camera> camera = param.sceneMgr->m_camera;
		std::shared_ptr<World>  world = param.world;

		auto vertexBuf = resource->find(DATA_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(DATA_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		if (!(lightEntity = world->getEntity(World::ENTITY_POINT_LIGHT)) || !lightEntity->hasComp<LightComp>())
		{
			goto coloring_without_light;
		}

		LightComp &lightComp = lightEntity->getComp<LightComp>();

		if (!lightComp.m_light)
		{
			goto coloring_without_light;
		}

		//>>>>>>> coloring with light begin
		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHTED_MESH, param.rhi)))
		{
			return;
		}

		shader->use(true);

		modelMat = part->getTransform();
		normalMat = MathLib::normalMat(modelMat);

		shader->setVec3("u_viewPos", camera->getCamPos());

		shader->setMat4("u_modelMat", modelMat);
		shader->setMat4("u_viewMat", param.sceneMgr->m_camera->getViewMat());
		shader->setMat4("u_prjMat", param.sceneMgr->m_frustum->getPerspectMat());
		shader->setMat4("u_normalMat", normalMat);

		lightComp.m_light->dealShader(shader);

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

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when drawing part");

		vertexBuf->unbind();
		indiceBuf->unbind();
		if (texture) texture->unbind();

		shader->use(false);

		lightRenderEntity = lightComp.m_entity;
		if (lightRenderEntity)
			drawLightCaster(lightRenderEntity, param);

		return;
		//>>>>>>> coloring with light end

	coloring_without_light:
		return;
	}

	void DrawCmdGL::drawLightCaster(std::shared_ptr<RenderEntity> &entity, RenderParam &param)
	{
		for (auto &part : entity->m_parts)
		{
			drawLightCasterPart(part, param);
		}

		if (!entity->m_children.empty())
		{
			for (auto &child : entity->m_children)
			{
				drawLightCaster(child, param);
			}
		}
	}

	void DrawCmdGL::drawLightCasterPart(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}
		std::shared_ptr<GfxBuffer>  texture{ nullptr };
		std::shared_ptr<Shader>     shader{ nullptr };

		std::shared_ptr<RenderResourceMgr> resource = param.resource;

		auto vertexBuf = resource->find(DATA_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(DATA_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		if (!(shader = param.shaderMgr->get(SHADER_TYPE_LIGHT_CASTER, param.rhi)))
		{
			return;
		}

		shader->use(true);

		shader->setMat4("u_modelMat", part->getTransform());
		shader->setMat4("u_viewMat", param.sceneMgr->m_camera->getViewMat());
		shader->setMat4("u_prjMat", param.sceneMgr->m_frustum->getPerspectMat());

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

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when drawing light caster's part");

		vertexBuf->unbind();
		indiceBuf->unbind();
		if (texture) texture->unbind();

		shader->use(false);
	}
}