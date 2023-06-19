#include "DepthPass.h"

#include "function/framework/world/World.h"

#include "function/framework/comp/RenderComp.h"
#include "function/framework/comp/LightComp.h"
#include "function/framework/comp/OcclusionComp.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/geo/ScreenRender.h"

#include "function/render/resource/RenderResourceMgr.h"
#include "function/render/resource/buffer/GfxFrameBuffer.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DepthPass"

namespace Pionner
{
	DepthPass::DepthPass() : RenderPassBase()
		, m_depthFboWidth(1024), m_depthFboHeight(1024)
		, m_depthFbo(nullptr), m_depthScreenRender(nullptr)
	{
	}

	DepthPass::~DepthPass()
	{
		m_depthFbo.reset();
		m_depthScreenRender.reset();
	}

	void DepthPass::initialize(const RenderPassInitInfo &info)
	{
		m_rhi = info.rhi;
		//m_depthScreenRender = std::make_shared<ScreenRender>(SHADER_TYPE_DEPTH_SCREEN_2D);
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
		auto resource = param.resource;
		const RenderViewport &port = param.renderViewport;

		if (!m_depthFbo)
		{
			m_depthFbo = resource->allocFbo(BUF_DEPTH_FRAMEBUFFER);
			m_depthFbo->setSize(m_depthFboWidth, m_depthFboHeight);
		}

		m_depthFbo->upload();

		m_depthFbo->bind();
		rhi->clear(DEPTH_BUF_BIT);
		//uint32_t l = port.m_left + port.m_width / 2 - m_depthFboWidth / 2;
		//uint32_t t = port.m_top + port.m_height / 2 - m_depthFboHeight / 2;
		rhi->setViewport(0, 0, m_depthFboWidth, m_depthFboHeight);

		DepthTest depthTest{};
		depthTest.m_enbale = true;
		rhi->setDepthMode(depthTest);

		world->iterate([&](decs::EntityID id, RenderComp &comp0, OcclusionComp &comp1)
		{
			cmd->drawDepth(*comp0.m_entity.get(), param);
		});

		depthTest.m_enbale = false;
		rhi->setDepthMode(depthTest);

		m_depthFbo->unbind();

		if (m_depthScreenRender)
		{
			m_depthScreenRender->setTextureId(m_depthFbo->getAttachment(DEPTH_ATTACH));
			m_depthScreenRender->draw(param);
		}
	}
}