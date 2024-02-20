#ifndef __PIONNER_GFX_RENDERER_RENDERER_H__
#define __PIONNER_GFX_RENDERER_RENDERER_H__

#include "gfx/rhi/RhiDef.h"
#include "gfx/renderer/RenderData.h"

#include "core/CommandQueue.h"
#include "platform/SystemUtils.h"
#include "scene/SceneDef.h"

namespace pio
{
	class RenderThread;
	class MaterialTable;
	class RenderAPI;
	class Texture2D;
	class CubeTexture;
	class UniformBufferSet;
	class RenderPass;
	class FrameBuffer;
	struct Viewport;

	class RenderListener
	{
	public:
		virtual void onRenderInit() = 0;
		virtual void onRenderDestroy() = 0;
	};

	class Renderer
	{
	public:
		// ------------- public static constant -------------
		static const glm::vec4 TRANSPARENT_COLOR;
		// --------------------------------------------------

	public:
		static void Init();
		static void Shutdown();
		static void SetRenderListener(RenderListener *listener) { s_listener = listener; }
		static RendererConfig &GetConfig() { return s_config; }

		// --------------------------------------------------------------------
		static void SwapCmdQueues();
		// Submmit task which will be executed before render cmds are run
		template<typename FuncT>
		static void SubmitTask(FuncT &&func)
		{
			auto task = [](void *ptr)
			{
				auto pFunc = (FuncT *)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderTaskQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit render cmds into render thread
		template<typename FuncT>
		static void SubmitRC(FuncT &&func)
		{
			auto renderCmd = [](void *ptr)
			{
				auto pFunc = (FuncT *)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCmdQueue().allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		// Submmit garbage collection task which will be executed after render cmds are run
		template<typename FuncT>
		static void SubmitGC(FuncT &&func)
		{
			auto task = [](void *ptr)
			{
				auto pFunc = (FuncT *)ptr;
				(*pFunc)();

				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderGarbageQueue().allocate(task, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		// --------------------------------------------------------------------

		static void BeginFrame();
		static void EndFrame();

		static void RenderLoop(RenderThread *thread);

		static bool IsRenderThread() { return SystemUtils::GetThreadId() == s_data->ThreadId; }
		static uint64_t GetThreadId() { return s_data->ThreadId; }
		static Ref<Texture2D> GetWhiteTexture() { return s_data->WhiteTexture; }
		static Ref<Texture2D> GetBlackTexture() { return s_data->BlackTexture; }

		static void CommitViewport(const Viewport &viewport);

		static void FramebufferBlockTransfer(uint32_t readBuffer, uint32_t dstBuffer, 
											 glm::ivec2 srcPos, glm::ivec2 srcSize,
											 glm::ivec2 dstPos, glm::ivec2 dstSize,
											 FrameBufferBlockBit flags, TextureFilterMag filter);

		static void RenderSubmesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
								  Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);		

		static void RenderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
										     Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		static void RenderPointLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
									       Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);

		static void RenderLightVolume(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &modelMat, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state);

		// ---------------------------- Deferring shading ----------------------------
		static void RenderSubmesh_Deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable,
										   Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state);
		static void RenderPointLightQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state);
		static void RenderDistantLightingQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state);
		// ---------------------------------------------------------------------------

		// ------------------------------ 2D Rendering -------------------------------
		static void RenderLine(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state);
		static void RenderTextureQuad2D(AssetHandle &meshHandle, Ref<Texture2D> &texture, const RenderState &state);
		static void RenderSprite(const AssetHandle &quadMesh, const AssetHandle &texture, const RenderState &state);
		// ---------------------------------------------------------------------------

		// ------------------------------- Tools -------------------------------------
		static void RenderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state);
		static void RenderHDRToCube(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment cubeAttach, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &cubeFbo);
		static void Postprocessing(AssetHandle &meshHandle, Ref<Texture2D> &composite, const RenderState &state);
		// ---------------------------------------------------------------------------

		static void BeginRenderPass(Ref<RenderPass> &pass);
		static void EndRenderPass(Ref<RenderPass> &pass);		

		static bool UI_HasFocus(UiFocus focus);

	private:
		struct GlobalRenderData
		{
			Ref<Texture2D> WhiteTexture{ nullptr };
			Ref<Texture2D> BlackTexture{ nullptr };
			uint64_t ThreadId{ 0 };

			~GlobalRenderData();
			void init();
		};

		static CommandQueue &GetRenderCmdQueue();
		static CommandQueue &GetRenderTaskQueue();
		static CommandQueue &GetRenderGarbageQueue();
		static uint32_t GetSubmissionIndex();
		static uint32_t GetRenderQueueIndex();

	private:
		static void InitRenderAPI();
		static void ShutdownRenderAPI();
		static void WaitAndRender(RenderThread *thread);

		static RenderAPI *s_API;
		static GlobalRenderData *s_data;
		static RenderListener *s_listener;
		static RendererConfig s_config;
	};
}

#endif