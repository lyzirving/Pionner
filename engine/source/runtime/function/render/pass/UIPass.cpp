#include "function/render/pass/UIPass.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/scene/SceneMgr.h"

#include "function/render/WindowSystem.h"

#include "function/ui/WindowUI.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UIPass"

namespace Pionner
{
	UIPass::UIPass() : RenderPassBase()
		, m_ui(nullptr)
		, m_windowWidth(0), m_windowHeight(0)
	{
	}

	UIPass::~UIPass()
	{
		m_ui.reset();
	}

	void UIPass::initialize(const RenderPassInitInfo &info)
	{
		m_rhi = info.rhi;
		m_windowWidth = info.window->getWidth();
		m_windowHeight = info.window->getHeight();
	}

	void UIPass::shutdown()
	{
		m_ui.reset();
		m_rhi.reset();
	}

	ViewLayout UIPass::getRenderportLayout()
	{
		ViewLayout layout{};
		std::shared_ptr<WindowView> view{ nullptr };
		if (m_ui && (view = m_ui->getView(UID_RENDER_PORT)))
		{
			ViewLayout port = view->getLayout();
			port.m_top = m_windowHeight - port.m_top - port.m_height;
			return port;
		}
		LOG_ERR("fail to get renderport layout");
		return layout;
	}

	void UIPass::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
	{
		m_ui = ui;
		m_rhi->initUIRenderBackend();
	}

	void UIPass::shutdownUIRenderBackend()
	{
		m_rhi->shutdownUIRenderBackend();
	}

	void UIPass::resize(int width, int height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		if (m_ui)
			m_ui->resize(width, height);
	}

	void UIPass::draw(RenderParam &param)
	{
		if (m_ui)
			m_ui->draw(param);
	}
}
