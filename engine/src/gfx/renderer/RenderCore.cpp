#include "RenderCore.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderCore"

namespace pio
{
	PIO_SINGLETON_IMPL(RenderCore)

	RenderCore::RenderCore() 
	{
	}

	RenderCore::~RenderCore() = default;

	void RenderCore::Init()
	{
		RenderCore::Get();
	}

	void RenderCore::Shutdown()
	{
		std::lock_guard<std::mutex> lk{ s_mutex };
		delete s_instance;
		s_instance = nullptr;
	}

	void RenderCore::renderLoop()
	{
		while(m_thread.isRunning())
		{
			waitAndRender();
		}
	}

	void RenderCore::waitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_thread.waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		// task before render cmd
		m_taskQueue[queueIdx()].execute();
		// rendering
		m_cmdQueue[queueIdx()].execute();
		// garbage collection
		m_garbageQueue[queueIdx()].execute();

		// finish waiting, begin execute draw cmd
		m_thread.set(RenderThread::State::Idle);
	}
}