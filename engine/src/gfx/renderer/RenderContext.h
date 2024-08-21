#ifndef __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__
#define __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__

#include "RenderThread.h"
#include "PendingData.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/rhi/UniformBuffer.h"

#include "base/utils/SystemUtils.h"
#include "base/CommandQueue.h"

namespace pio
{
	class Window;

	class RenderContext
	{
	public:	
		RenderContext(BackendFlags type, Ref<Window> &window);
		~RenderContext() = default;

		void renderLoop();

		BackendFlags backendFlag() const { return m_api->type(); }
		uint64_t frame() const { return m_frameNum; }
		PendingData& pendingData() { return m_pendingData; }
		RenderThread& thread() { return m_thread; }
		Ref<Window>&  window() { return m_window; }
		
		void setUpPendingData(PendingData&& data) { m_pendingData = std::forward<PendingData>(data); }
		void swapQueues() { m_submitIdx = (m_submitIdx + 1) % k_queueNum; }
		bool bInRenderThread() const { return m_thread.isRunning() && SystemUtils::GetThreadId() == m_threadId; }

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

		void recyleResource(ResourceGCDelegate &&data) 
		{			
			auto &api = m_api;
			submitGC([data, &api]() mutable { api->releaseResource(std::move(data)); });
		}	

		void uploadData(void* data, uint32_t size, Ref<UniformBuffer>& buffer)
		{
			submitTask([data, size, &buffer]() mutable { buffer->setData(data, size, 0); });
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
		Ref<RenderAPI> m_api;		
		Ref<Window> m_window;
		RenderThread m_thread;
		uint64_t m_threadId{ 0 };

		std::atomic<uint32_t> m_submitIdx{ 0 };
		CommandQueue m_cmdQueue[k_queueNum];
		CommandQueue m_taskQueue[k_queueNum];
		CommandQueue m_garbageQueue[k_queueNum];

		PendingData m_pendingData;

		uint64_t m_frameNum{ 0 };
	};
}

#endif