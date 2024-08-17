#include "Editor.h"

#include "window/Window.h"

#include "event/AppEvent.h"

#include "event/EventBus.h"
#include "event/EventHub.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace pio
{
	Editor *Editor::k_Editor = nullptr;

	Editor *Editor::Get() { return k_Editor; }

	Editor::Editor() : EventHubObj()
	{
		k_Editor = this;

		LogSystem::Initialize();
		EventBus::Init();		
	}

	Editor::~Editor()
	{
		EventHub::Shutdown();
		EventBus::Shutdown();
		LogSystem::Shutdown();
	}

	void Editor::onInit()
	{		 
		EventHub::Get()->registerCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));

		WindowProps prop{ "Pionner", 1400, 720, Backend_OpenGL };
		m_window = Window::create(prop);		

		m_renderCtx = CreateRef<RenderContext>(Backend_OpenGL, m_window);
	}

	void Editor::onQuit()
	{		
		LOGD("enter");
		EventHub::Get()->removeCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));		
		m_renderCtx.reset();
		m_window.reset();
	}

	void Editor::onEvent(Ref<Event> &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<WindowCloseEvent>(PIO_BIND_FN_SELF(Editor::onWindowClose, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		dispatcher.dispatch<WindowResizeEvent>(PIO_BIND_FN_SELF(Editor::onWindowResize, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);
	}

	bool Editor::onWindowClose(Ref<WindowCloseEvent> &event)
	{
		LOGD("window is closed");
		m_running = false;
		return true;
	}

	bool Editor::onWindowResize(Ref<WindowResizeEvent> &event)
	{
		return false;
	}

	void Editor::run()
	{
		onInit();

		auto &renderThread = m_renderCtx->thread();
		renderThread.run(PIO_BIND_FN_OTHER(RenderContext::renderLoop, m_renderCtx.get()));
		// Block until the first frame has been done
		renderThread.pump();

		while(m_running)
		{
			//------- Wait for render thread to finish commands -------
			// Wait for render thread to finish renderering
			renderThread.blockUntilRenderComplete();
			renderThread.nextFrame();
			m_renderCtx->swapQueues();
			// Start rendering previous frame in render thread
			renderThread.kick();
			//---------------------------------------------------------
			
			EventBus::Get()->dispatch();
			EventHub::Get()->dispatch();
		}

		renderThread.terminate();

		onQuit();
	}
}