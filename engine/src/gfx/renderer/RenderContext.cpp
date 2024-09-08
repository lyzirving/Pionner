#include "RenderContext.h"

#include "window/Window.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/FrameBuffer.h"
#include "gfx/rhi/UniformBuffer.h"
#include "gfx/rhi/ShaderCompiler.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderContext"

namespace pio
{
	RenderContext::RenderContext(RenderBackendFlags flag, Ref<Window>& window) : m_window(window)
	{
		m_api = RenderAPI::Create(flag);
		m_state = RenderState::Create(flag);
	}

	void RenderContext::renderLoop()
	{
		m_threadId = SystemUtils::GetThreadId();
		LOGD("enter render thread[%lu]", m_threadId);

		// Initialization
		m_window->init();
		m_window->makeCurrent();
		m_window->setVSync(true);		
		m_api->setupBackend();
		m_api->setupUiBackend(m_window->nativeWindow());

		initResource();

		LOGD("enter render loop");
		while (m_thread.isRunning())
		{
			m_window->pollEvents();

			onWindowSizeChange(0, 0, m_window->width(), m_window->height());

			waitAndRender();

			m_window->swapBuffer();

			m_frameNum++;
		}
		LOGD("exit render thread[%lu]", m_threadId);
		//Call before garbage queue is executed
		releaseResource();
		// Clear resources submitted at the last frame
		m_garbageQueue[submitIdx()].execute();

		m_api->shutdown();
		m_window->shutdown();
		m_thread.set(RenderThread::State::Idle);
	}

	void RenderContext::onBeginFrameRendering()
	{
	}

	void RenderContext::onEndFrameRendering()
	{
		std::swap(m_renderingEntities, RenderingEntities());
		std::swap(m_renderingData, RenderingData());
	}

	void RenderContext::onBeginFrameBuffer(Ref<FrameBuffer>& frameBuffer,const RenderStateAttrs& attrs)
	{		
		m_api->setViewport(0, 0, frameBuffer->width(), frameBuffer->height());
		frameBuffer->bind();
		m_state->setStateMachine(attrs);
	}

	void RenderContext::onEndFrameBuffer(Ref<FrameBuffer>& frameBuffer)
	{
		frameBuffer->unbind();		
		m_api->setViewport(m_vp.offsetX(), m_vp.offsetY(), m_vp.ratioW(), m_vp.ratioH());// restore viewport
	}

	bool RenderContext::bindUnimBlock(Ref<Shader>& shader, Ref<UniformBuffer>& unimBuff, const std::string& blockName)
	{
		return m_api->bindUnimBlock(shader->id(), unimBuff->binding(), blockName);
	}

	void RenderContext::drawTriangles(Ref<VertexArray>& vao, Ref<IndexBuffer>& ebo)
	{
		vao->bind();
		ebo->bind();

		m_api->drawElements(DrawMode_Triangle, ebo->indexCount(), ebo->internalFmt());

		ebo->unbind();
		vao->unbind();
	}

	void RenderContext::initResource()
	{
		auto context = self();
		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i] = ShaderCompiler::Compile(context, ShaderUtils::GetShaderPath(ShaderType(i)));
		}
		m_textureMgr.init(context);
	}

	void RenderContext::releaseResource()
	{
		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i].reset();//Real release will be executed at garbage queue
		}
		m_textureMgr.release();
	}

	void RenderContext::waitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_thread.waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		m_api->onBeginFrame(*this);

		// task before render cmd
		m_taskQueue[queueIdx()].execute();
		// rendering
		m_cmdQueue[queueIdx()].execute();
		// garbage collection
		m_garbageQueue[queueIdx()].execute();

		m_api->onEndFrame(*this);

		// finish waiting, begin execute draw cmd
		m_thread.set(RenderThread::State::Idle);
	}

	void RenderContext::onWindowSizeChange(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		if(m_vp.w() != w || m_vp.h() != h || 
		   m_vp.x() != x || m_vp.y() != y)
		{
			m_vp.setX(x);
			m_vp.setY(y);
			m_vp.setW(w);
			m_vp.setH(h);
		}
	}
}