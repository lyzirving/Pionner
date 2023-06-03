#include <cassert>

#include "Editor.h"
#include "EditorUI.h"

#include "view/RenderView.h"
#include "view/LeftPanelView.h"
#include "view/RightPanelView.h"
#include "view/BottomPanelView.h"

#include "Engine.h"
#include "function/global/GlobalContext.h"
#include "function/render/RenderSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace Pionner
{
	Editor::Editor(const std::shared_ptr<Engine> &engine)
		: m_runtimeEngine(engine)
		, m_ui(nullptr)
	{
	}

	Editor::~Editor() = default;

	void Editor::initialize()
	{
		if (!m_runtimeEngine)
			assert(0);

		m_ui = std::make_shared<EditorUI>();
		WindowUIInitInfo uiInitInfo;
		uiInitInfo.windowSystem = g_runtimeCtx.m_windowSystem;
		uiInitInfo.renderSystem = g_runtimeCtx.m_renderSystem;
		m_ui->initialize(uiInitInfo);

		auto pUi = m_ui->getPtr();

		std::shared_ptr<WindowView> render = std::shared_ptr<WindowView>(new RenderView);
		render->attachParent(pUi);
		std::shared_ptr<WindowView> leftPanel = std::shared_ptr<WindowView>(new LeftPanelView);
		leftPanel->attachParent(pUi);
		std::shared_ptr<WindowView> rightPanel = std::shared_ptr<WindowView>(new RightPanelView);
		rightPanel->attachParent(pUi);
		std::shared_ptr<WindowView> bottomPanel = std::shared_ptr<WindowView>(new BottomPanelView);
		bottomPanel->attachParent(pUi);

		m_ui->addView(render);
		m_ui->addView(leftPanel);
		m_ui->addView(rightPanel);
		m_ui->addView(bottomPanel);

		m_ui->layout();

		g_runtimeCtx.m_renderSystem->initializeUIRenderBackend(pUi);

		LOG_DEBUG("finish initialization");
	}

	void Editor::shutdown()
	{
		if (m_ui)
		{
			g_runtimeCtx.m_renderSystem->shutdownUIRenderBackend();
			m_ui->shutdown();
			m_ui.reset();
		}
		m_runtimeEngine.reset();
	}

	void Editor::run()
	{
		if (!m_runtimeEngine)
		{
			LOG_FATAL("engine is null");
			assert(0);
		}
		while (true)
		{
			float delta = m_runtimeEngine->calculateDeltaTime();
			if (!m_runtimeEngine->tickFrame(delta))
				return;
		}
	}
}