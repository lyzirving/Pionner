#include "PointShadowScreenRender.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/GeometryComp.h"
#include "function/framework/comp/LightComp.h"
#include "function/framework/comp/ShadowComp.h"

#include "function/render/RenderDef.h"
#include "function/render/rhi/Rhi.h"

#include "function/render/resource/ResourceDef.h"
#include "function/render/scene/SceneMgr.h"
#include "function/render/resource/buffer/GfxBuffer.h"
#include "function/render/resource/RenderResourceMgr.h"

#include "function/render/shader/ShaderMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PointShadowScreenRender"

namespace Pionner
{
	PointShadowScreenRender::PointShadowScreenRender() 
		: ScreenRender(SHADER_TYPE_PT_SHADOW_SCREEN, "pt shadow screen render")
	{
	}

	PointShadowScreenRender::~PointShadowScreenRender() = default;

	void PointShadowScreenRender::draw(RenderParam &param)
	{
		if (m_texId == 0)
		{
			LOG_ERR("pt shadow screen render has invalid texture id");
			return;
		}

		auto scene = param.sceneMgr;
		auto light = scene->m_lights[scene->m_curLight];

		if (!light || light->type() != LIGHT_TYPE_POINT)
		{
			LOG_ERR("invalid light type");
			return;
		}

		std::shared_ptr<Shader> shader{ nullptr };
		if (!dealShader(param, shader))
		{
			LOG_ERR("pt shadow screen render fail to deal shader");
			return;
		}

		auto rhi = param.rhi;
		auto world = param.world;
		auto cmd = rhi->getDrawCmd();
		const RenderViewport &port = param.renderViewport;

		rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);
		rhi->clear(COLOR_BUF_BIT | DEPTH_BUF_BIT);

		DepthTest depthtest = DepthTest::common();
		rhi->setDepthMode(depthtest);

		// temporary buffer that holds an initialized buffer
		auto holdertex = param.resource->createHolderBuffer(BUF_CUBE_TEXTURE);
		holdertex->setHolderId(m_texId);

		shader->use(true);

		holdertex->bindTarget(0);
		shader->setInt("u_pointDepthMap", 0);
		shader->setFloat("u_farPlane", scene->m_frustum->far());

		world->iterate([&](decs::EntityID id, ShadowComp &comp)
		{
			auto entity = world->getEntity(id.index);
		if (entity)
		{
			if (entity->hasComp<RenderComp>())
			{
				auto &renderComp = entity->getComp<RenderComp>();
				cmd->drawEntityOnly(*renderComp.m_entity.get(), param);
			}
		}
		});

		shader->use(false);
	}

	bool PointShadowScreenRender::dealShader(RenderParam &param, std::shared_ptr<Shader> &shader)
	{
		shader = param.shaderMgr->get(m_shaderType, param.rhi);
		return shader && shader->isInit();
	}
}