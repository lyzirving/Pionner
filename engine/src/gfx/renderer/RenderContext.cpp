#include "RenderContext.h"

#include "window/Window.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderContext"

namespace pio
{
	RenderContext::RenderContext(CRenderApiType type, Ref<Window> &window) : m_window(window)
	{
		m_api = CRenderAPI::Create(type);
	}

	void RenderContext::initialize()
	{
		m_api->init();
	}

	void RenderContext::shutdown()
	{
		m_api->shutdown();
	}

	void RenderContext::renderLoop()
	{
		LOGD("enter render thread");
		initialize();

		while (m_thread.isRunning())
		{
			waitAndRender();
		}

		shutdown();
	}

	void RenderContext::waitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_thread.waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		// garbage collection
		m_garbageQueue[queueIdx()].execute();
		// task before render cmd
		m_taskQueue[queueIdx()].execute();
		// rendering
		m_cmdQueue[queueIdx()].execute();

		// finish waiting, begin execute draw cmd
		m_thread.set(RenderThread::State::Idle);
	}
}