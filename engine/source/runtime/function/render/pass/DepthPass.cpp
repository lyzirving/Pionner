#include "DepthPass.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/GeometryComp.h"
#include "function/framework/comp/LightComp.h"
#include "function/framework/comp/ShadowComp.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/rhi/Rhi.h"

#include "function/render/geo/screen/DepthScreenRender.h"
#include "function/render/shader/ShaderMgr.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxFrameBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DepthPass"

namespace Pionner
{
	DepthPass::DepthPass() : RenderPassBase(), m_depthScreenRender(nullptr)
	{
	}

	DepthPass::~DepthPass()
	{
		m_depthScreenRender.reset();
	}

	void DepthPass::initialize(const RenderPassInitInfo &info)
	{
		m_rhi = info.rhi;
		//m_depthScreenRender = std::shared_ptr<ScreenRender>(new DepthScreenRender);
	}

	void DepthPass::shutdown()
	{
		m_rhi.reset();
	}

	void DepthPass::draw(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto cmd = rhi->getDrawCmd();
		auto world = param.world;
		std::shared_ptr<Shader> shader{ nullptr };
		const RenderViewport &port = param.renderViewport;

		auto scene = param.sceneMgr;
		auto light = scene->m_lights[scene->m_curLight];
		std::shared_ptr<GfxFrameBuffer> depthBuf{ nullptr };

		if (!light || !(depthBuf = light->getDepthFbo()) || depthBuf->getWidth() == 0 || depthBuf->getHeight() == 0)
		{
			LOG_ERR("no light in the screen, or light's buffer is invalid");
			return;
		}

		depthBuf->upload();

		depthBuf->bind();

		rhi->clear(DEPTH_BUF_BIT);
		rhi->setViewport(0, 0, depthBuf->getWidth(), depthBuf->getHeight());

		DepthTest depthTest{};
		depthTest.m_enbale = true;
		rhi->setDepthMode(depthTest);

		world->iterate([&](decs::EntityID id, ShadowComp &comp)
		{
			auto entity = world->getEntity(id.index);
		if (entity)
		{
			if (entity->hasComp<RenderComp>())
			{
				auto &renderComp = entity->getComp<RenderComp>();
				cmd->drawDepth(*renderComp.m_entity.get(), param);
			}
			else if (entity->hasComp<GeometryComp>())
			{
				auto &geoComp = entity->getComp<GeometryComp>();
				geoComp.m_geometry->initialize(param);
				if (geoComp.m_geometry->dealDepthShader(param, shader))
				{
					cmd->drawGeometry(*geoComp.m_geometry.get(), param);
					shader->use(false);
				}
			}
		}
		});

		depthTest.m_enbale = false;
		rhi->setDepthMode(depthTest);

		depthBuf->unbind();

		if (m_depthScreenRender)
		{
			m_depthScreenRender->setTextureId(depthBuf->getAttachment(DEPTH_ATTACH));
			m_depthScreenRender->draw(param);
		}
	}
}