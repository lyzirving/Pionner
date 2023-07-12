#include "GraphicContext.h"

#include "global/window/WindowSystem.h"

#include "core/log/LogSystem.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GraphicContext"

namespace pio
{
	namespace gfx
	{
		GraphicContext::GraphicContext() : m_state(new State), m_rhi(new Rhi)
		{
		}

		GraphicContext::~GraphicContext() = default;

		bool GraphicContext::init(const GfxContextInitParam &param)
		{
			if (!m_rhi->init())
			{
				LOG_ERR("Rhi init failed");
				return false;
			}

			if (!m_rhi->initUiBackend(param.window))
			{
				return false;
			}

			if (!m_state->init())
			{
				LOG_ERR("State init failed");
				return false;
			}

			return true;
		}

		void GraphicContext::shutdown()
		{
			if (m_state)
			{
				m_state->shutdown();
				m_state.reset();
			}

			if (m_rhi)
			{
				m_rhi->shutdownUiBackend();
				m_rhi->shutdown();
				m_rhi.reset();
			}
		}
	}
}