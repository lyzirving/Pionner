#include "UIPass.h"

#include "render/rhi/Rhi.h"
#include "render/scene/SceneMgr.h"

#include "global/window/WindowSystem.h"

#include "global/window/ui/WindowUI.h"
#include "global/event/EventMgr.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UIPass"

namespace pio
{
	UIPass::UIPass() : RenderPassBase()
		, m_ui(nullptr)
		, m_windowWidth(0), m_windowHeight(0)
		, m_renderLayout()
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

	void UIPass::initializeUIRenderBackend(const std::shared_ptr<WindowUI> &ui)
	{
		m_ui = ui;
		m_rhi->initUIRenderBackend();
		calcLayout();
	}

	bool UIPass::dealEvent(RenderParam &param, const Event &evt)
	{
		if (!m_ui)
			return false;

		if (evt.m_type == EVENT_TYPE_NONE)
		{
			m_ui->resetTargetView();
			return false;
		}

		if ((m_renderLayout.contains(evt.m_posX, evt.m_posY)))
		{
			if (m_ui->dealEvent(param, evt))
				return true;
		}
		m_ui->resetTargetView();

		return false;
	}

	void UIPass::shutdownUIRenderBackend()
	{
		m_rhi->shutdownUIRenderBackend();
	}

	void UIPass::resize(int width, int height)
	{
		m_windowWidth = width;
		m_windowHeight = height;
		if (m_ui) m_ui->setWndSize(width, height);
		calcLayout();
	}

	void UIPass::calcLayout()
	{
		if (m_windowWidth <= 0 || m_windowHeight <= 0)
		{
			LOG_ERR("invalid window size[%d, %d]", m_windowWidth, m_windowHeight);
			return;
		}
		std::shared_ptr<WindowView> view{ nullptr };
		if (m_ui && (view = m_ui->getView(UID_RENDER_PORT)))
		{
			m_renderLayout = view->getLayout();
		}
		else
		{
			LOG_ERR("fail to get render view");
		}
	}

	void UIPass::draw(RenderParam &param)
	{
		
	}
}
