#include "function/render/rhi/opengl/cmd/DrawCmdGL.h"
#include "function/render/rhi/RhiHeader.h"
#include "function/render/rhi/opengl/GLHelper.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/shader/ShaderMgr.h"
#include "function/render/shader/Shader.h"

#include "function/render/entity/RenderEntity.h"
#include "function/render/RenderDef.h"

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
				DrawCmdGL::drawEntity(child, param);
			}
		}
	}

	void DrawCmdGL::drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}
		std::shared_ptr<RenderResourceMgr> resource = param.resource;
		auto vertexBuf = resource->find(DATA_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(DATA_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}
		auto shader = param.shaderMgr->get(SHADER_TYPE_MESH, param.rhi);

		if (!shader)
		{
			LOG_ERR("mesh shader is invalid");
			return;
		}

		shader->use(true);
		// TODO: add vertex array attribute in shader
		shader->setMat4("u_viewMat", param.camera->getViewMat());
		shader->setMat4("u_prjMat", param.frustum->getProjectMat());
		shader->setMat4("u_modelMat", part->m_modelMat);

		vertexBuf->upload();
		indiceBuf->upload();
		GLHelper::checkGLErr("err happens after upload");

		vertexBuf->bindToTarget(0);
		indiceBuf->bind();

		if (part->m_material.slotValid())
		{
			auto texture = resource->find(DATA_TEXTURE, part->m_material.m_slot);
			if (texture)
			{
				texture->upload();
				texture->bindToTarget(part->m_partIndex);
				std::string sampler = (part->m_material.m_type == MAT_DIFFUSE) ? "u_diffuse" : "u_spec";
				shader->setInt(sampler, part->m_partIndex);
			}
		}

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens in draw cmd");

		shader->use(false);
	}
}