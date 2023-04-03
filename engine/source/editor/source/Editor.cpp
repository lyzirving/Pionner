#include <cassert>

#include "Editor.h"

#include "Engine.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace Pionner
{
	Editor::Editor(const std::shared_ptr<Engine>& engine) 
		: m_runtimeEngine(engine)
	{
	}

	Editor::~Editor() = default;

	void Editor::initialize()
	{
		if (!m_runtimeEngine)
			assert(0);

		LOG_DEBUG("finish initialization");
	}

	void Editor::shutdown()
	{
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