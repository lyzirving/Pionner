#ifndef __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__
#define __PIONNER_GFX_RENDERER_RENDER_CONTEXT_H__

#include "RenderThread.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/rhi/RenderState.h"

#include "gfx/renderer/resource/RenderingData.h"

#include "base/utils/SystemUtils.h"
#include "base/CommandQueue.h"

namespace pio
{
	class Window;
	class Shader;
	class FrameBuffer;

	class RenderContext : public std::enable_shared_from_this<RenderContext>
	{
	public:	
		RenderContext(RenderBackendFlags flag, Ref<Window> &window);
		~RenderContext() = default;

		void renderLoop();

		RenderBackendFlags renderBackend() const { return m_api->renderBackend(); }
		uint64_t frame() const { return m_frameNum; }
		const Viewport &vp() const { return m_vp; }

		RenderingEntities& renderingEntities() { return m_renderingEntities; }
		RenderingData& renderingData() { return m_renderingData; }
		Ref<RenderContext> self() { return shared_from_this(); }
		RenderThread& thread() { return m_thread; }
		Ref<Window>& window() { return m_window; }
		Ref<RenderState>& state() { return m_state; }
		
		Ref<Shader> &shader(ShaderType type) { return m_shaders[PIO_UINT8(type)]; }
		void setRenderingEntities(RenderingEntities&& data) { m_renderingEntities = std::forward<RenderingEntities>(data); }
		void setRenderingData(RenderingData &&data) { m_renderingData = std::forward<RenderingData>(data); }
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

		template<typename Type>
		void uploadData(void* data, uint32_t size, Ref<Type>& resource)
		{
			submitTask([data, size, &resource]() mutable { resource->setData(data, size, 0); });
		}

		template<typename Type>
		void uploadData(Ref<Type> &resource)
		{
			submitTask([&resource]() mutable { resource->init(); });
		}

		void onBeginFrameRendering();
		void onEndFrameRendering();

		void onBeginFrameBuffer(Ref<FrameBuffer>& frameBuffer, const RenderStateAttrs& attrs);
		void onEndFrameBuffer(Ref<FrameBuffer>& frameBuffer);

		bool bindUnimBlock(Ref<Shader> &shader, uint32_t bindingPt, const std::string &blockName);

	private:
		static constexpr uint32_t k_queueNum = 2;		

	protected:
		void initShaders();
		void releaseShaders();
		void waitAndRender();

		void onWindowSizeChange(int32_t x, int32_t y, int32_t w, int32_t h);

		uint32_t submitIdx() const { return m_submitIdx; }
		uint32_t queueIdx()  const { return (m_submitIdx + 1) % k_queueNum; }

		CommandQueue& cmdQueue() { return m_cmdQueue[m_submitIdx]; }
		CommandQueue& taskQueue() { return m_taskQueue[m_submitIdx]; }
		CommandQueue& garbageQueue() { return m_garbageQueue[m_submitIdx]; }

	private:
		Ref<RenderAPI> m_api;
		Ref<RenderState> m_state;

		Ref<Window> m_window;
		Viewport m_vp;
		RenderThread m_thread;
		uint64_t m_threadId{ 0 };

		std::atomic<uint32_t> m_submitIdx{ 0 };
		CommandQueue m_cmdQueue[k_queueNum];
		CommandQueue m_taskQueue[k_queueNum];
		CommandQueue m_garbageQueue[k_queueNum];		

		uint64_t m_frameNum{ 0 };

		Ref<Shader> m_shaders[PIO_UINT8(ShaderType::Num)];

		RenderingEntities m_renderingEntities;
		RenderingData m_renderingData;
	};
}

#endif