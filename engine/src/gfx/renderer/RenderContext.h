#ifndef __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__
#define __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__

#include "RenderThread.h"
#include "core/CommandQueue.h"

namespace pio
{
	class RenderContext
	{
	public:	
		RenderContext() {}
		~RenderContext() = default;

		void init();
		void shutdown();
		void renderLoop();

		RenderThread& thread() { return m_thread; }
		void swapQueues() { m_submitIdx = (m_submitIdx + 1) % k_queueNum; }

		// Submmit garbage collection task which will be executed before the 
		// execution of task and render cmds
		template<typename FuncT>
		void submitGC(FuncT&& func)
		{
			auto task = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};
			auto storageBuffer = garbageQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit task which will be executed before render cmds are run
		template<typename FuncT>
		void submitTask(FuncT&& func)
		{
			auto task = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = taskQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit render cmds into render thread
		template<typename FuncT>
		void submitRC(FuncT&& func)
		{
			auto renderCmd = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			auto storageBuffer = cmdQueue().allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

	private:
		static constexpr uint32_t k_queueNum = 2;

	protected:
		void waitAndRender();

		uint32_t submitIdx() const { return m_submitIdx; }
		uint32_t queueIdx()  const { return (m_submitIdx + 1) % k_queueNum; }

		CommandQueue& cmdQueue() { return m_cmdQueue[m_submitIdx]; }
		CommandQueue& taskQueue() { return m_taskQueue[m_submitIdx]; }
		CommandQueue& garbageQueue() { return m_garbageQueue[m_submitIdx]; }

	private:
		RenderThread m_thread;

		std::atomic<uint32_t> m_submitIdx{ 0 };
		CommandQueue m_cmdQueue[k_queueNum];
		CommandQueue m_taskQueue[k_queueNum];
		CommandQueue m_garbageQueue[k_queueNum];
	};
}

#endif