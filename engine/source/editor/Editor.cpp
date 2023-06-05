#include <cassert>

#include "Editor.h"
#include "EditorUI.h"

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
		std::shared_ptr<WindowUI> uiSelf = m_ui->getPtr();
		m_ui->createUI(uiSelf);
		m_ui->layout();

		g_runtimeCtx.m_renderSystem->initializeUIRenderBackend(uiSelf);

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