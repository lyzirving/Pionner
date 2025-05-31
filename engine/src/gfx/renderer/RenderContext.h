#pragma once

#include "RenderThread.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/rhi/RenderState.h"
#include "gfx/rhi/RenderResource.h"
#include "gfx/pipeline/TransientData.h"

namespace pio
{
	class Window;
	class Shader;

	class FrameBuffer;
	class UniformBuffer;
	class MeshRenderBuffer;	

	class Renderer;
	class Texture;		
	class TextureMgr;
	class ShaderLab;
	struct TextureSpecific;

	class RenderContext : public Object
	{
		RTTR_ENABLE(Object)
	public:	
		RenderContext(RenderBackendFlags flag, Ref<Window> &window);
		~RenderContext() = default;

		void RenderLoop();

		RenderBackendFlags RenderBackend() const { return m_Api->RenderBackend(); }
		uint64_t Frame() const { return m_FrameNum; }
		const Viewport& GetViewport() const { return m_Viewport; }		

		RenderingData& GetRenderingData() { return m_RenderingData; }
		const RenderingData& GetRenderingData() const { return m_RenderingData; }		
		void ClearRenderingData() { std::swap(m_RenderingData, RenderingData()); }

		Ref<Renderer>& GetRenderer() { return m_Renderer; }
		RenderThread& Thread() { return m_Thread; }
		Ref<Window>& GetWindow() { return m_Window; }		
		Ref<RenderState>& State() { return m_State; }
		void SwapQueues() { m_SubmitIdx = (m_SubmitIdx + 1) % k_QueueNum; }		

		bool IsRenderThread() const { return m_Thread.IsRunning() && SystemUtil::GetThreadID() == m_ThreadID; }

		// Submmit garbage collection task which will be executed before the 
		// execution of task and render cmds
		template<typename FuncT>
		void SubmitGC(FuncT&& func)
		{
			auto task = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = GarbageQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit task which will be executed before render cmds are run
		template<typename FuncT>
		void SubmitTask(FuncT&& func)
		{
			auto task = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
				pFunc->~FuncT();
			};
			auto storageBuffer = TaskQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit render cmds into render thread
		template<typename FuncT>
		void SubmitRC(FuncT&& func)
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
			auto storageBuffer = CmdQueue().allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		void RecyleResource(ResourceGCDelegate &&data) 
		{			
			auto &api = m_Api;
			SubmitGC([data, &api]() mutable { api->ReleaseResource(std::move(data)); });
		}	

		template<typename Type>
		void UploadData(void* data, uint32_t size, Ref<Type>& resource)
		{
			SubmitTask([data, size, &resource]() mutable { resource->SetData(data, size, 0); });
		}

		template<typename Type>
		void UploadData(Ref<Type>& resource)
		{
			SubmitTask([&resource]() mutable { resource->Init(); });
		}

		template<typename Type>
		void UploadIndice(void* data, uint32_t size, uint32_t indiceNum, Ref<Type>& resource)
		{
			SubmitTask([data, size, indiceNum, &resource]() mutable { resource->SetIndice(data, size, indiceNum, 0); });
		}

		void OnBeginFrameRendering();
		void OnEndFrameRendering();

		void OnBeginFrameBuffer(const Ref<FrameBuffer>& frameBuffer, const RenderStateAttrs& attrs);
		void OnBeginFrameBuffer(const Ref<FrameBuffer>& frameBuffer, const RenderStateAttrs& attrs, uint8_t depthSlot);
		void OnBindScreenFrameBuffer();
		void OnEndFrameBuffer(const Ref<FrameBuffer>& frameBuffer);

		bool BindUnimBlock(const Ref<Shader>& shader, const Ref<UniformBuffer>& unimBuff, const std::string& blockName);

		void DrawTriangles(const Ref<MeshRenderBuffer>& meshBuff);
		void DrawLines(const Ref<MeshRenderBuffer>& meshBuff);

		// ------------------------------ Render state api ------------------------------------		
		void SetClear(const Clear& clear) { m_State->SetClear(clear); }
		void SetCullFace(const CullFace& cull) { m_State->SetCullFace(cull); }
		void SetBlendMode(const Blend& blend) { m_State->SetBlendMode(blend); }
		void SetDepthTest(const DepthTest& depth) { m_State->SetDepthTest(depth); }
		void SetStencilTest(const StencilTest& stencil) { m_State->SetStencilTest(stencil); }
		// ------------------------------------------------------------------------------------

		// ----------------------------- Create resource api ----------------------------------
		Ref<Texture> CreateTexture(const TextureSpecific& spec);
		/*
		* @param buffer Buffer is not marked as const, data inside the buffer will be moved into texture.
		*/
		Ref<Texture> CreateTexture(const TextureSpecific& spec, Buffer& buffer);
		Ref<Texture> CreateTexture(const TextureSpecific& spec, uint8_t** buffer);
		Ref<Texture> CreateTexture(const TextureSpecific& spec, const std::string& path);
		Ref<Texture> GetTexture(const std::string& name);
		
		Ref<Shader> FindShader(ShaderSpecifier spec);	
		// ------------------------------------------------------------------------------------
		Ref<MeshRenderBuffer> GetScreenMeshBuff() const { return m_ScreenMeshBuffer; }

	private:
		static constexpr uint32_t k_QueueNum = 2;		

	protected:
		void InitResource();
		void ReleaseResource();
		void WaitAndRender();

		void OnWindowSizeChange(int32_t x, int32_t y, int32_t w, int32_t h);

		uint32_t SubmitIdx() const { return m_SubmitIdx; }
		uint32_t QueueIdx()  const { return (m_SubmitIdx + 1) % k_QueueNum; }

		CommandQueue& CmdQueue() { return m_CmdQueue[m_SubmitIdx]; }
		CommandQueue& TaskQueue() { return m_TaskQueue[m_SubmitIdx]; }
		CommandQueue& GarbageQueue() { return m_GarbageQueue[m_SubmitIdx]; }

	private:
		Ref<RenderAPI> m_Api;
		Ref<RenderState> m_State;
		Ref<Renderer> m_Renderer;

		Ref<Window> m_Window;
		Viewport m_Viewport;
		RenderThread m_Thread;
		uint64_t m_ThreadID{ 0 };
		uint32_t m_BindFbo{ 0 };

		std::atomic<uint32_t> m_SubmitIdx{ 0 };
		CommandQueue m_CmdQueue[k_QueueNum];
		CommandQueue m_TaskQueue[k_QueueNum];
		CommandQueue m_GarbageQueue[k_QueueNum];		

		uint64_t m_FrameNum{ 0 };

		Ref<TextureMgr> m_TextureMgr;
		Ref<ShaderLab> m_ShaderLab;

		RenderingData m_RenderingData;
		Ref<MeshRenderBuffer> m_ScreenMeshBuffer;
	};
}