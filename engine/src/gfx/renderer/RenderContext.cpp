#include "RenderContext.h"

#include "window/Window.h"

#include "gfx/rhi/Shader.h"
#include "gfx/rhi/ShaderCompiler.h"

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
		onWindowSizeChange(m_window->width(), m_window->height());
		m_api->setupBackend();
		m_api->setupUiBackend(m_window->nativeWindow());

		initShaders();

		while (m_thread.isRunning())
		{
			m_window->pollEvents();

			waitAndRender();

			m_window->swapBuffer();

			m_frameNum++;
		}
		LOGD("exit render thread[%lu]", m_threadId);
		//Call before garbage queue is executed
		releaseShaders();
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

	void RenderContext::onWindowSizeChange(uint32_t w, uint32_t h)
	{
		m_viewport.setX(0);
		m_viewport.setY(0);
		m_viewport.setW(w);
		m_viewport.setH(h);
	}

	void RenderContext::initShaders()
	{
		auto context = self();
		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i] = ShaderCompiler::Compile(context, ShaderUtils::GetShaderPath(ShaderType(i)));
		}
	}

	void RenderContext::releaseShaders()
	{
		for (uint8_t i = 0; i < PIO_UINT8(ShaderType::Num); i++)
		{
			m_shaders[i].reset();//Real release will be executed at garbage queue
		}
	}

	void RenderContext::waitAndRender()
	{
		// Wait for kick, then set render thread to busy
		m_thread.waitAndSet(RenderThread::State::Kick, RenderThread::State::Busy);

		m_api->beginFrame(*this);

		// task before render cmd
		m_taskQueue[queueIdx()].execute();
		// rendering
		m_cmdQueue[queueIdx()].execute();
		// garbage collection
		m_garbageQueue[queueIdx()].execute();

		m_api->endFrame(*this);

		// finish waiting, begin execute draw cmd
		m_thread.set(RenderThread::State::Idle);
	}
}