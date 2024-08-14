#ifndef __PIONNER_GFX_RENDERER_RENDER_CORE_H__
#define __PIONNER_GFX_RENDERER_RENDER_CORE_H__

#include "core/CommandQueue.h"
#include "gfx/GfxDef.h"

#include "RenderThread.h"

namespace pio
{
	class RenderCore
	{
		PIO_SINGLETON_DECLARE(RenderCore)
	public:
		RenderThread &thread() { return m_thread; }
		void swapQueues() { m_submitIdx = (m_submitIdx + 1) % k_queueNum; }

		void renderLoop();			

	protected:
		void waitAndRender();
		
		uint32_t submitIdx() const { return m_submitIdx; }
		uint32_t queueIdx()  const { return (m_submitIdx + 1) % k_queueNum; }

		CommandQueue &cmdQueue()     { return m_cmdQueue[m_submitIdx]; }
		CommandQueue &taskQueue()    { return m_taskQueue[m_submitIdx]; }
		CommandQueue &garbageQueue() { return m_garbageQueue[m_submitIdx]; }

	private:
		static constexpr uint32_t k_queueNum = 2;

	private:
		RenderThread m_thread;

		std::atomic<uint32_t> m_submitIdx{ 0 };
		CommandQueue m_cmdQueue[k_queueNum];
		CommandQueue m_taskQueue[k_queueNum];
		CommandQueue m_garbageQueue[k_queueNum];
	};
}

#endif