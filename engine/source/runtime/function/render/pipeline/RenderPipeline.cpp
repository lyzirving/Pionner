#include "function/render/pipeline/RenderPipeline.h"

#include "function/render/scene/SceneMgr.h"
#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/scene/Layout.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/pass/UIPass.h"

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
		RenderPassCommonInfo passCommon;
		passCommon.rhi = m_rhi;

		m_uiPass = std::make_shared<UIPass>();
		m_uiPass->setPassCommonInfo(passCommon);

		RenderPassInitInfo passInitInfo;
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
			auto layout = param.sceneMgr->m_layout;
			m_rhi->viewportSub(layout->m_drawPanelInfo.m_left,
							   layout->m_windowHeight - layout->m_drawPanelInfo.m_height,
							   layout->m_drawPanelInfo.m_width,
							   layout->m_drawPanelInfo.m_height);
			scene->forwardRender(param);
		}

		m_rhi->viewportFull();
		m_uiPass->draw(sceneMgr);
	}

	void RenderPipeline::preparePassData(RenderParam &param)
	{
		auto rhi = param.rhi;
		auto windowSys = rhi->getWindowSystem();
		auto sceneMgr = param.sceneMgr;

		if (windowSys->sizeChange())
		{
			int width = windowSys->getWidth();
			int height = windowSys->getHeight();

			if (m_uiPass)
				m_uiPass->resize(width, height);
			sceneMgr->resize(width, height);
			rhi->reviseViewport(width, height);
			windowSys->setSizeChange(false);
		}
	}
}