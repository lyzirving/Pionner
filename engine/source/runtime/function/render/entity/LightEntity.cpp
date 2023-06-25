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
		m_type = RENDER_ENTITY_TYPE_LIGHT;
	}

	LightEntity::~LightEntity() = default;

	bool LightEntity::dealShader(RenderParam &param, std::shared_ptr<EntityPart> &part,
								 /*out*/std::shared_ptr<Shader> &shader,
								 /*out*/std::shared_ptr<GfxBuffer> &texture)
	{
		if (!(shader = param.shaderMgr->get(SHADER_TYPE_MESH, param.rhi)))
		{
			return false;
		}
		// TODO: to be implemented.
		return true;
	}

	bool LightEntity::dealDepthShader(RenderParam &param, std::shared_ptr<EntityPart> &part, std::shared_ptr<Shader> &shader)
	{
		return true;
	}
}