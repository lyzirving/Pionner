#include <glm/gtc/matrix_transform.hpp>

#include "function/render/rhi/opengl/cmd/DrawCmdGL.h"
#include "function/render/rhi/RhiHeader.h"
#include "function/render/rhi/opengl/GLHelper.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"

#include "function/render/scene/SceneMgr.h"

#include "function/render/shader/ShaderMgr.h"

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

	void DrawCmdGL::drawEntity(RenderEntity &entity, RenderParam &param)
	{
		for (auto &part : entity.m_parts)
		{
			drawPart(part, param);
		}

		if (!entity.m_children.empty())
		{
			for (auto &child : entity.m_children)
			{
				drawEntity(*child, param);
			}
		}
	}

	void DrawCmdGL::drawGeometry(Geometry &geometry, RenderParam &param)
	{
		auto meshComp = geometry.getMeshComp();
		if (!meshComp || !meshComp->m_initialized || meshComp->m_vBufSlot < 0 || meshComp->m_indBufSlot < 0)
		{
			LOG_ERR("mesh comp is invalid");
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

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		GLHelper::checkGLErr("err happens when color geometry");

		vertexBuf->unbind();
		indiceBuf->unbind();
	}

	void DrawCmdGL::drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		auto owner = part->m_owner;

		if (!owner)
			return;

		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}
		std::shared_ptr<GfxBuffer> texture{ nullptr };
		std::shared_ptr<Shader>    shader{ nullptr };

		std::shared_ptr<RenderResourceMgr> resource = param.resource;

		auto vertexBuf = resource->find(DATA_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(DATA_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		if (!owner->dealShader(param, part, shader, texture))
		{
			LOG_ERR("fail to deal shader");
			return;
		}

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
	}
}