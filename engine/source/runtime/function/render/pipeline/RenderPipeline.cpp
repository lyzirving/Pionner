#include "RenderPipeline.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/pass/UIPass.h"
#include "function/render/pass/DepthPass.h"

#include "function/render/WindowSystem.h"
#include "function/event/EventMgr.h"

#include "core/system/PlatformUtil.h"
#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace Pionner
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
		if (m_uiPass) m_uiPass->initializeUIRenderBackend(ui);
	}

	void RenderPipeline::shutdownUIRenderBackend()
	{
		if (m_uiPass) m_uiPass->shutdownUIRenderBackend();
	}

	void RenderPipeline::shutdown()
	{
		m_uiPass->shutdown();
		m_depthPass->shutdown();
	}

	void RenderPipeline::forwardRender(RenderParam &param)
	{
		ViewLayout port = m_uiPass->getRenderViewport();
		// move viewport to renderport
		m_rhi->viewportSub(port.m_left, port.m_top, port.m_width, port.m_height);

		m_depthPass->draw(param);
		param.sceneMgr->m_scene->forwardRender(param);

		// move viewport to full screen
		m_rhi->viewportFull();

		m_uiPass->draw(param);
	}

	void RenderPipeline::preparePassData(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto windowSystem = rhi->getWindowSystem();
		auto sceneMgr = param.sceneMgr;
		std::shared_ptr<EventMgr> evtMgr{ nullptr };

		// deal window size-change
		if (windowSystem->sizeChange())
		{
			int width = windowSystem->getWidth();
			int height = windowSystem->getHeight();

			if (m_uiPass) m_uiPass->resize(width, height);
			sceneMgr->resize(width, height);
			rhi->reviseViewport(width, height);
			windowSystem->setSizeChange(false);
		}

		if (m_uiPass && (evtMgr = windowSystem->getEvtMgr()))
		{
			Event evt = evtMgr->processEvent();
			m_uiPass->dealEvent(param, evt);
		}
	}
}