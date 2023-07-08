#include "RenderPipeline.h"

#include "render/scene/SceneMgr.h"
#include "render/scene/RenderScene.h"
#include "render/scene/Camera.h"
#include "render/scene/Frustum.h"

#include "render/rhi/Rhi.h"
#include "render/pass/UIPass.h"
#include "render/pass/DepthPass.h"

#include "global/window/WindowSystem.h"
#include "global/event/EventMgr.h"

#include "core/system/PlatformUtil.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	RenderPipeline::RenderPipeline() : m_rhi(nullptr), m_uiPass(nullptr), m_depthPass(nullptr)
	{
	}

	RenderPipeline::~RenderPipeline()
	{
		m_depthPass.reset();
		m_uiPass.reset();
		m_rhi.reset();
	}

	void RenderPipeline::initialize(RenderPipelineInitInfo &info)
	{
		m_uiPass = std::make_shared<UIPass>();
		m_depthPass = std::make_shared<DepthPass>();

		RenderPassInitInfo passInitInfo;
		passInitInfo.rhi = m_rhi;
		passInitInfo.window = info.window;

		m_uiPass->initialize(passInitInfo);
		m_depthPass->initialize(passInitInfo);
	}

	void RenderPipeline::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
	{
		m_uiPass->initializeUIRenderBackend(ui);
	}

	void RenderPipeline::shutdownUIRenderBackend()
	{
		m_uiPass->shutdownUIRenderBackend();
	}

	void RenderPipeline::shutdown()
	{
		m_uiPass->shutdown();
		m_depthPass->shutdown();
	}

	void RenderPipeline::forwardRender(RenderParam &param)
	{
		m_depthPass->draw(param);

		param.sceneMgr->m_scene->forwardRender(param);

		m_uiPass->draw(param);
	}

	void RenderPipeline::preparePassData(RenderParam &param)
	{
		auto windowSystem = param.rhi->getWindowSystem();

		const ViewLayout &layout = m_uiPass->getRenderLayout();
		param.renderViewport.m_width = layout.m_width;
		param.renderViewport.m_height = layout.m_height;
		param.renderViewport.m_left = layout.m_left;
		param.renderViewport.m_top = windowSystem->getHeight() - layout.m_top - layout.m_height;

		param.windowViewport.m_left = param.windowViewport.m_height = 0;
		param.windowViewport.m_width = windowSystem->getWidth();
		param.windowViewport.m_height = windowSystem->getHeight();
	}

	void RenderPipeline::swapData(RenderParam &param)
	{
		auto windowSystem = param.rhi->getWindowSystem();

		// deal window size-change
		if (windowSystem->sizeChange())
		{
			int width = windowSystem->getWidth();
			int height = windowSystem->getHeight();

			m_uiPass->resize(width, height);
			param.sceneMgr->resize(width, height);
			windowSystem->setSizeChange(false);
		}

		// deal motion event
		std::shared_ptr<EventMgr> evtMgr{ nullptr };
		if (m_uiPass && (evtMgr = windowSystem->getEvtMgr()))
		{
			Event evt = evtMgr->processEvent();
			m_uiPass->dealEvent(param, evt);
		}
	}
}