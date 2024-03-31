#include "Renderer.h"
#include "Renderer.h"
#include "Renderer.h"
#include "RenderThread.h"

#include "asset/AssetsManager.h"
#include "platform/opengl/GLRenderAPI.h"

#include "gfx/rhi/RenderAPI.h"
#include "gfx/rhi/Texture.h"
#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/rhi/RenderPass.h"

#include "gfx/struct/ShaderLibrary.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Camera.h"
#include "gfx/struct/Geometry2D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Renderer"

namespace pio
{
	const glm::vec4 Renderer::TRANSPARENT_COLOR{ glm::vec4(0.f) };

	RenderAPI *Renderer::s_API{ nullptr };
	Renderer::GlobalRenderData *Renderer::s_data{ nullptr };
	RenderListener *Renderer::s_listener{ nullptr };
	RendererConfig Renderer::s_config{};

	static constexpr uint32_t s_queueCnt = 2;
	static CommandQueue *s_cmdQueue[s_queueCnt];
	static CommandQueue *s_taskQueue[s_queueCnt];
	static CommandQueue *s_garbageQueue[s_queueCnt];
	// record index of queue that is used to receive cmd from outside
	static std::atomic<uint32_t> s_submissionIndex = 0;

	void Renderer::Init()
	{
		s_cmdQueue[0] = new CommandQueue;
		s_cmdQueue[1] = new CommandQueue;

		s_taskQueue[0] = new CommandQueue;
		s_taskQueue[1] = new CommandQueue;

		s_garbageQueue[0] = new CommandQueue;
		s_garbageQueue[1] = new CommandQueue;

		s_data = new GlobalRenderData;
		s_data->init();

		GetConfig().FullScreenQuad = MeshFactory::CreateFullScreenQuad()->getHandle();
	}

	void Renderer::Shutdown()
	{
		s_listener = nullptr;

		delete s_cmdQueue[0];
		delete s_cmdQueue[1];
		delete s_taskQueue[0];
		delete s_taskQueue[1];
		delete s_garbageQueue[0];
		delete s_garbageQueue[1];

		delete s_data;
		s_data = nullptr;
	}

	CommandQueue &Renderer::GetRenderCmdQueue()
	{
		return *s_cmdQueue[s_submissionIndex];
	}

	CommandQueue &Renderer::GetRenderTaskQueue()
	{
		return *s_taskQueue[s_submissionIndex];
	}

	CommandQueue &Renderer::GetRenderGarbageQueue()
	{
		return *s_garbageQueue[s_submissionIndex];
	}

	uint32_t Renderer::GetSubmissionIndex()
	{
		return s_submissionIndex;
	}

	uint32_t Renderer::GetRenderQueueIndex()
	{
		return (s_submissionIndex + 1) % s_queueCnt;
	}

	void Renderer::SwapCmdQueues()
	{
		s_submissionIndex = (s_submissionIndex + 1) % s_queueCnt;
	}

	void Renderer::BeginFrame()
	{
		s_API->beginFrame();
	}

	void Renderer::EndFrame()
	{
		s_API->endFrame();
	}

	void Renderer::RenderLoop(RenderThread *thread)
	{
		if (s_listener) { s_listener->onRenderInit(); }

		InitRenderAPI();

		s_data->ThreadId = SystemUtils::GetThreadId();

		while (thread->isRunning())
		{
			WaitAndRender(thread);
		}

		if (s_listener) { s_listener->onRenderDestroy(); }

		ShutdownRenderAPI();

		LOGD("exit render loop");
	}

	void Renderer::CommitViewport(const Viewport &viewport)
	{
		s_API->commitViewport(viewport);
	}

	void Renderer::FramebufferBlockTransfer(uint32_t readBuffer, uint32_t dstBuffer, glm::ivec2 srcPos, glm::ivec2 srcSize, glm::ivec2 dstPos, glm::ivec2 dstSize, FrameBufferBlockBit flags, TextureFilterMag filter)
	{
		s_API->framebufferBlockTransfer(readBuffer, dstBuffer, srcPos, srcSize, dstPos, dstSize, flags, filter);
	}

	void Renderer::RenderSubmesh(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass,
								 Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		s_API->renderSubmesh(meshHandle, submeshIndex, materialTable, shadowPass, uniformBufferSet, modelMat, state);
	}

	void Renderer::RenderDistantLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		s_API->renderDistantLightShadow(meshHandle, submeshIndex, isRigged, materialTable, shadowPass, uniformBufferSet, modelMat, state);
	}

	void Renderer::RenderPointLightShadow(AssetHandle &meshHandle, uint32_t submeshIndex, bool isRigged, Ref<MaterialTable> &materialTable, Ref<RenderPass> &shadowPass, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		s_API->renderPointLightShadow(meshHandle, submeshIndex, isRigged, materialTable, shadowPass, uniformBufferSet, modelMat, state);
	}

	void Renderer::RenderLightVolume(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &modelMat, Ref<UniformBufferSet> &uniformBufferSet, const RenderState &state)
	{
		s_API->renderLightVolume(meshHandle, submeshIndex, modelMat, uniformBufferSet, state);
	}

	void Renderer::RenderSubmesh_Deferred(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<MaterialTable> &materialTable, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &modelMat, const RenderState &state)
	{
		s_API->renderSubmesh_deferred(meshHandle, submeshIndex, materialTable, uniformBufferSet, modelMat, state);
	}

	void Renderer::RenderPointLightQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state)
	{
		s_API->renderPointLightQuad_Deferred(meshHandle, uniformBufferSet, GBufferPass, shadowPass, state);
	}

	void Renderer::RenderDistantLightingQuad_Deferred(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const Ref<RenderPass> &GBufferPass, const Ref<RenderPass> &shadowPass, const RenderState &state)
	{
		s_API->renderDistantLightingQuad_Deferred(meshHandle, uniformBufferSet, GBufferPass, shadowPass, state);
	}

	void Renderer::RenderLine(AssetHandle &meshHandle, Ref<UniformBufferSet> &uniformBufferSet, const glm::mat4 &trans, const RenderState &state)
	{
		s_API->renderLine(meshHandle, uniformBufferSet, trans, state);
	}

	void Renderer::RenderTextureQuad2D(AssetHandle &meshHandle, Ref<Texture2D> &texture, const RenderState &state)
	{
		s_API->renderTextureQuad2D(meshHandle, texture, state);
	}

	void Renderer::RenderSprite(const AssetHandle &quadMesh, const AssetHandle &texture, const RenderState &state)
	{
		s_API->renderSprite(quadMesh, texture, state);
	}

	void Renderer::RenderSkybox(AssetHandle &meshHandle, uint32_t submeshIndex, Ref<UniformBufferSet> &uniformBufferSet, Ref<CubeTexture> &cubeTexture, const RenderState &state)
	{
		s_API->renderSkybox(meshHandle, submeshIndex, uniformBufferSet, cubeTexture, state);
	}

	void Renderer::RenderHDRToEnvMap(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment envMapAttachment, Ref<Texture2D> &HDRTexture, Ref<FrameBuffer> &fbo)
	{
		s_API->renderHDRToEnvMap(meshHandle, submeshIndex, prjMat, viewMat, state, envMapAttachment, HDRTexture, fbo);
	}

	void Renderer::RenderDiffuseConvolution(AssetHandle &meshHandle, uint32_t submeshIndex, const glm::mat4 &prjMat, const glm::mat4 viewMat[LightDir_Num], const RenderState &state, ColorAttachment diffuseAttachment, Ref<CubeTexture> &envMap, Ref<FrameBuffer> &fbo)
	{
		s_API->renderDiffuseConvolution(meshHandle, submeshIndex, prjMat, viewMat, state, diffuseAttachment, envMap, fbo);
	}

	void Renderer::Postprocessing(const AssetHandle &meshHandle, Ref<Texture2D> &composite, const RenderState &state)
	{
		s_API->postprocessing(meshHandle, composite, state);
	}

	void Renderer::BeginRenderPass(Ref<RenderPass> &pass)
	{
		s_API->beginRenderPass(pass);
	}

	void Renderer::EndRenderPass(Ref<RenderPass> &pass)
	{
		s_API->endRenderPass(pass);
	}

	void Renderer::BeginScreenPass(Ref<RenderPass> &pass, const Viewport &vp)
	{
		s_API->beginScreenPass(pass, vp);
	}

	void Renderer::EndScreenPass(Ref<RenderPass> &pass)
	{
		s_API->endScreenPass(pass);
	}

	bool Renderer::UI_HasFocus(UiFocus focus)
	{
		return s_API->UI_hasFocus(focus);
	}

	void Renderer::InitRenderAPI()
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				s_API = new GLRenderAPI;
				break;
			default:
				break;
		}
		s_API->init();

		ShaderLibrary::Init();
	}

	void Renderer::ShutdownRenderAPI()
	{
		ShaderLibrary::Shutdown();

		if (s_API)
		{
			s_API->shutdown();
			delete s_API;
			s_API = nullptr;
		}
	}

	void Renderer::WaitAndRender(RenderThread *thread)
	{
		// Wait for kick, then set render thread to busy
		thread->waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		// task before render cmd
		s_taskQueue[GetRenderQueueIndex()]->execute();
		// rendering
		s_cmdQueue[GetRenderQueueIndex()]->execute();
		// garbage collection
		s_garbageQueue[GetRenderQueueIndex()]->execute();

		// finish waiting, begin execute draw cmd
		thread->set(RenderThread::State::Idle);
	}

	Renderer::GlobalRenderData::~GlobalRenderData()
	{
		WhiteTexture.reset();
		BlackTexture.reset();
	}

	void Renderer::GlobalRenderData::init()
	{
		if (!WhiteTexture)
		{
			TextureSpecification spec;
			spec.Format = ImageInternalFormat::RGBA;
			spec.Name = "White Texture";
			spec.Width = 1;
			spec.Height = 1;
			auto buffer = Texture::MakeFlatBuffer(spec, 255);
			WhiteTexture = Texture2D::Create(spec, buffer);
		}

		if (!BlackTexture)
		{
			TextureSpecification spec;
			spec.Format = ImageInternalFormat::RGBA;
			spec.Name = "Black Texture";
			spec.Width = 1;
			spec.Height = 1;
			auto buffer = Texture::MakeFlatBuffer(spec, 0);
			BlackTexture = Texture2D::Create(spec, buffer);
		}
	}
}