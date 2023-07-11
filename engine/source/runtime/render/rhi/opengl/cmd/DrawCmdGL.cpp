#include <glm/gtc/matrix_transform.hpp>

#include "render/rhi/opengl/cmd/DrawCmdGL.h"
#include "render/rhi/RhiHeader.h"
#include "render/rhi/Rhi.h"

#include "render/resource/RenderResourceMgr.h"
#include "render/resource/buffer/GfxBuffer.h"

#include "render/scene/SceneMgr.h"

#include "render/shader/ShaderMgr.h"

#include "render/entity/RenderEntity.h"
#include "render/RenderDef.h"

#include "world/comp/MeshComp.h"
#include "world/comp/TransformComp.h"
#include "world/comp/LightComp.h"

#include "world/World.h"

#include "render/geo/Geometry.h"

#include "core/math/MathLib.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DrawCmdGL"

namespace pio
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

	void DrawCmdGL::drawDepth(RenderEntity &entity, RenderParam &param)
	{
		for (auto &part : entity.m_parts)
		{
			drawPartDepth(part, param);
		}

		if (!entity.m_children.empty())
		{
			for (auto &child : entity.m_children)
			{
				drawDepth(*child, param);
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
		auto vertexBuf = resource->find(BUF_VERTEX, meshComp->m_vBufSlot);
		auto indiceBuf = resource->find(BUF_INDICE, meshComp->m_indBufSlot);

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
		gfx::GLHelper::checkGLErr("err happens when color geometry");

		vertexBuf->unbind();
		indiceBuf->unbind();
	}

	void DrawCmdGL::drawGeometryDepth(Geometry &geometry, RenderParam &param)
	{
		std::shared_ptr<Shader> shader{ nullptr };
		geometry.initialize(param);
		if (geometry.dealDepthShader(param, shader))
		{
			drawGeometry(geometry, param);
			shader->use(false);
		}
	}

	bool DrawCmdGL::drawIdxTriangle(uint32_t idxSize)
	{
		glDrawElements(GL_TRIANGLES, idxSize, GL_UNSIGNED_INT, nullptr);
		return gfx::GLHelper::checkGLErr("err happens when drawing indexed triangles");
	}

	void DrawCmdGL::drawPart(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		auto owner = part->m_owner;
		auto scene = param.sceneMgr;
		std::shared_ptr<Light> light{ nullptr };
		if (scene->lightExist()) { light = scene->selectedLight(); }

		if (!owner)
			return;

		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}
		std::shared_ptr<GfxBuffer> texture{ nullptr };
		std::shared_ptr<Shader>    shader{ nullptr };

		auto resource = param.resource;
		auto vertexBuf = resource->find(BUF_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(BUF_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		if (!owner->dealShader(param, part, shader))
		{
			LOG_ERR("fail to deal shader");
			return;
		}

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		bool success = gfx::GLHelper::checkGLErr("err happens when drawing part");
		if (!success)
		{
			LOG_ERR("draw failed, part's owener name[%s], part index[%u], material name[%s]", 
					part->m_owner->m_name.c_str(), part->m_partIndex, part->m_material.m_name.c_str());
		}

		vertexBuf->unbind();
		indiceBuf->unbind();

		if (light) { light->unbind(param); }
		param.rhi->unbindBufSlot(BUF_TEXTURE);

		shader->use(false);
	}

	void DrawCmdGL::drawPartDepth(std::shared_ptr<EntityPart> &part, RenderParam &param)
	{
		auto owner = part->m_owner;

		if (!owner)
			return;

		if (!part->vetexSlotValid() || !part->indiceSlotValid())
		{
			return;
		}

		auto resource = param.resource;
		auto vertexBuf = resource->find(BUF_VERTEX, part->m_vertexSlot);
		auto indiceBuf = resource->find(BUF_INDICE, part->m_indicesSlot);

		if (!vertexBuf || !indiceBuf)
		{
			LOG_ERR("buffer is invalid");
			return;
		}

		std::shared_ptr<Shader> shader{ nullptr };

		if (!owner->dealDepthShader(param, part, shader))
		{
			LOG_ERR("fail to deal shader");
			return;
		}

		vertexBuf->upload();
		indiceBuf->upload();

		vertexBuf->bind();
		indiceBuf->bind();

		glDrawElements(GL_TRIANGLES, indiceBuf->size(), GL_UNSIGNED_INT, nullptr);
		gfx::GLHelper::checkGLErr("err happens when drawing part's depth");

		vertexBuf->unbind();
		indiceBuf->unbind();

		shader->use(false);
	}
}