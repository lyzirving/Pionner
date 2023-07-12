#include <cassert>

#include "Editor.h"
#include "EditorUI.h"

#include "Engine.h"
#include "global/GlobalContext.h"
#include "render2/RenderSystem.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace pio
{
	Editor::Editor(const std::shared_ptr<Engine> &engine)
		: m_engine(engine)
		, m_ui(nullptr)
	{
	}

	Editor::~Editor() = default;

	void Editor::initialize()
	{
		if (!m_engine)
			assert(0);

		m_ui = std::make_shared<EditorUI>();
		WindowUIInitInfo uiInitInfo{ g_runtimeCtx.m_windowSystem };
		m_ui->initialize(uiInitInfo);
		std::shared_ptr<WindowUI> uiSelf = m_ui->selfPtr();
		m_ui->createUI(uiSelf);
		m_ui->layout();

		g_runtimeCtx.m_render->attachUi(uiSelf);

		LOG_DEBUG("finish initialization");
	}

	void Editor::shutdown()
	{
		if (m_ui)
		{
			m_ui->shutdown();
			m_ui.reset();
		}
		m_engine.reset();
	}

	void Editor::run()
	{
		if (!m_engine)
		{
			LOG_FATAL("engine is null");
			assert(0);
		}
		while (true)
		{
			uint64_t delta = m_engine->calculateDeltaTime();
			if (!m_engine->tickFrame(delta))
				return;
		}
	}
}