#include "RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderContext"

namespace pio
{
	void RenderContext::init()
	{
	}

	void RenderContext::shutdown()
	{
	}

	void RenderContext::renderLoop()
	{
		while (m_thread.isRunning())
		{
			waitAndRender();
		}
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