#include "RenderContext.h"
#include "MeshRenderBuffer.h"
#include "TextureMgr.h"
#include "ShaderLab.h"
#include "Renderer.h"

#include "GlobalSettings.h"

#include "window/Window.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/MeshFactory.h"

#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/Shader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderContext"

namespace pio
{
	RenderContext::RenderContext(RenderBackendFlags flag, Ref<Window>& window) : m_Window(window)
	{
		m_Api = RenderAPI::Create(flag);
		m_State = RenderState::Create(flag);
	}

	void RenderContext::RenderLoop()
	{
		m_ThreadID = SystemUtil::GetThreadID();
		LOGD("enter render thread[%lu]", m_ThreadID);

		// Initialization
		m_Window->Init();
		m_Window->MakeCurrent();
		m_Window->SetVSync(true);		
		m_Api->SetupBackend();
		m_Api->SetupUiBackend(m_Window->NativeWindow());

		InitResource();

		LOGD("enter render loop");
		while (m_Thread.IsRunning())
		{
			m_Window->PollEvents();

			OnWindowSizeChange(0, 0, m_Window->Width(), m_Window->Height());

			WaitAndRender();

			m_Window->SwapBuffer();

			m_FrameNum++;
		}
		LOGD("exit render thread[%lu]", m_ThreadID);
		//Call before garbage queue is executed
		ReleaseResource();
		// Clear resources submitted at the last frame
		m_GarbageQueue[SubmitIdx()].execute();

		m_Api->Shutdown();
		m_Window->Shutdown();
		m_Thread.Set(RenderThread::State::Idle);
	}

	void RenderContext::OnBeginFrameRendering()
	{
	}

	void RenderContext::OnEndFrameRendering()
	{
		ClearRenderingData();

		SubmitRC([weakSelf = WeakRef<RenderContext>(Self<RenderContext>())]()
		{
			auto context = weakSelf.lock();
			if(!context)
				return;
			
			context->OnBindScreenFrameBuffer();
		});
	}

	void RenderContext::OnBeginFrameBuffer(const Ref<FrameBuffer>& frameBuffer,const RenderStateAttrs& attrs)
	{		
		if (m_BindFbo == frameBuffer->Id() && frameBuffer->Id() != 0)
		{
			m_State->ApplyStateChange(attrs);
			return;
		}		
		m_Api->SetViewport(0, 0, frameBuffer->Width(), frameBuffer->Height());
		frameBuffer->Bind();
		m_State->SetStateMachine(attrs);
		m_BindFbo = frameBuffer->Id();
	}

	void RenderContext::OnBeginFrameBuffer(const Ref<FrameBuffer>& frameBuffer, const RenderStateAttrs& attrs, uint8_t depthSlot)
	{
		m_Api->SetViewport(0, 0, frameBuffer->Width(), frameBuffer->Height());
		frameBuffer->BindWritingDepth(depthSlot);
		m_State->SetStateMachine(attrs);
		m_BindFbo = frameBuffer->Id();
	}

	void RenderContext::OnBindScreenFrameBuffer()
	{
		m_BindFbo = 0;
		m_Api->BindScreenFrameBuffer();
		m_Api->SetViewport(m_Viewport.offsetX(), m_Viewport.offsetY(), m_Viewport.ratioW(), m_Viewport.ratioH());// restore viewport
	}

	void RenderContext::OnEndFrameBuffer(const Ref<FrameBuffer>& frameBuffer)
	{
		if (frameBuffer)
			frameBuffer->UnBind();
		m_BindFbo = 0;
		m_Api->SetViewport(m_Viewport.offsetX(), m_Viewport.offsetY(), m_Viewport.ratioW(), m_Viewport.ratioH());// restore viewport
	}

	bool RenderContext::BindUnimBlock(const Ref<Shader>& shader, const Ref<UniformBuffer>& unimBuff, const std::string& blockName)
	{
		return m_Api->BindUnimBlock(shader->Id(), unimBuff->Binding(), blockName);
	}

	void RenderContext::DrawTriangles(const Ref<MeshRenderBuffer>& meshBuff)
	{
		meshBuff->Vao->Bind();
		meshBuff->Ebo->Bind();

		m_Api->DrawElements(DrawMode_Triangle, meshBuff->Ebo->IndexCount(), meshBuff->Ebo->InternalFmt());

		meshBuff->Ebo->UnBind();
		meshBuff->Vao->UnBind();
	}

	void RenderContext::DrawLines(const Ref<MeshRenderBuffer>& meshBuff)
	{
		meshBuff->Vao->Bind();
		meshBuff->Ebo->Bind();

		m_Api->DrawElements(DrawMode_Line, meshBuff->Ebo->IndexCount(), meshBuff->Ebo->InternalFmt());

		meshBuff->Ebo->UnBind();
		meshBuff->Vao->UnBind();
	}

	bool RenderContext::CopyFrameBufferDepth(const Ref<FrameBuffer>& src, const Ref<FrameBuffer>& dst)
	{
		return m_Api->CopyFrameBufferDepth(src, dst);
	}

	Ref<Texture> RenderContext::CreateTexture(const TextureSpecific& spec)
	{
		return m_TextureMgr->Create(spec);
	}

	Ref<Texture> RenderContext::CreateTexture(const TextureSpecific& spec, Buffer& buffer)
	{
		return m_TextureMgr->Create(spec, buffer);
	}

	Ref<Texture> RenderContext::CreateTexture(const TextureSpecific& spec, uint8_t** buffer)
	{
		return m_TextureMgr->Create(spec, buffer);
	}

	Ref<Texture> RenderContext::CreateTexture(const TextureSpecific& spec, const std::string& path)
	{
		return m_TextureMgr->Create(spec, path);
	}

	Ref<Texture> RenderContext::GetTexture(const std::string& name)
	{
		return m_TextureMgr->Get(name);
	}

	Ref<Shader> RenderContext::FindShader(ShaderSpecifier spec)
	{
		return m_ShaderLab->Get(spec);
	}

	void RenderContext::InitResource()
	{
		auto context = Self<RenderContext>();	

		m_TextureMgr = CreateRef<TextureMgr>(context);
		m_ShaderLab = CreateRef<ShaderLab>(context);

		m_Renderer = Renderer::Create(GlobalSettings::RenderConfig);
		m_Renderer->OnAttach(context);

		InitParam initParam;
		initParam.Context = context;
		AssetMgr::Init(initParam);

		auto screenSubMesh = MeshFactory::MakeScreenQuad();
		m_ScreenMeshBuffer = CreateRef<MeshRenderBuffer>();
		m_ScreenMeshBuffer->Setup(context, screenSubMesh.Vertice, screenSubMesh.Indice);
	}

	void RenderContext::ReleaseResource()
	{
		auto context = Self<RenderContext>();

		m_Renderer->OnDetach(context);
		m_ScreenMeshBuffer.reset();
		AssetMgr::Shutdown();
		m_TextureMgr->Release();
		m_ShaderLab->Release();
	}

	void RenderContext::WaitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_Thread.WaitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		m_Api->OnBeginFrame(*this);

		// task before render cmd
		m_TaskQueue[QueueIdx()].execute();
		// rendering
		m_CmdQueue[QueueIdx()].execute();
		// garbage collection
		m_GarbageQueue[QueueIdx()].execute();

		m_Api->OnEndFrame(*this);

		// finish waiting, begin execute draw cmd
		m_Thread.Set(RenderThread::State::Idle);
	}

	void RenderContext::OnWindowSizeChange(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		if(m_Viewport.w() != w || m_Viewport.h() != h || 
		   m_Viewport.x() != x || m_Viewport.y() != y)
		{
			m_Viewport.setX(x);
			m_Viewport.setY(y);
			m_Viewport.setW(w);
			m_Viewport.setH(h);
		}
	}
}