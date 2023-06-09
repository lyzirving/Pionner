#include "function/render/pipeline/RenderPipeline.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/pass/UIPass.h"

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
	RenderPipeline::RenderPipeline() : RenderPipelineBase()
	{
	}

	RenderPipeline::~RenderPipeline() = default;

	void RenderPipeline::initialize(RenderPipelineInitInfo &info)
	{
		m_uiPass = std::make_shared<UIPass>();
		RenderPassInitInfo passInitInfo;
		passInitInfo.rhi = m_rhi;
		passInitInfo.window = info.window;
		m_uiPass->initialize(passInitInfo);
	}

	void RenderPipeline::shutdownUIRenderBackend()
	{
		m_uiPass->shutdownUIRenderBackend();
	}

	void RenderPipeline::shutdown()
	{
		m_uiPass->shutdown();
	}

	void RenderPipeline::forwardRender(RenderParam &param)
	{
		if (!param.sceneMgr)
			return;

		auto sceneMgr = param.sceneMgr;
		auto scene = param.sceneMgr->m_scene;

		if (scene)
		{
			ViewLayout port = m_uiPass->getRenderViewport();
			m_rhi->viewportSub(port.m_left, port.m_top, port.m_width, port.m_height);
			scene->forwardRender(param);
		}

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
			// TODO: add a hit target
			m_uiPass->dealEvent(param, evt);
		}
	}
}