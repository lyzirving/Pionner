#include "Editor.h"

#include "window/Window.h"

#include "event/AppEvent.h"
#include "event/EventBus.h"
#include "event/EventHub.h"

#include "gfx/renderer/RenderContext.h"
#include "scene/Registry.h"
#include "scene/Components.h"

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

		Time::RecordTime();
		LogSystem::Initialize();
		EventBus::Init();		
	}

	Editor::~Editor()
	{
		EventHub::Shutdown();
		EventBus::Shutdown();
		LogSystem::Shutdown();
	}

	void Editor::onAttach()
	{		 
		Registry::Init();
		EventHub::Get()->registerCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));

		m_window = Window::create(WindowProps("Pionner", 1400, 720, Backend_OpenGL));
		m_renderCtx = CreateRef<RenderContext>(Backend_OpenGL, m_window);

		auto scene = CreateRef<Scene>();
		m_sceneMgr.add(scene);
	}

	void Editor::onDetach()
	{		
		LOGD("begin to destroy resource");
		auto& renderThread = m_renderCtx->thread();
		m_sceneMgr.removeAll();
		renderThread.terminate();
		LOGD("wake up from render thread");

		EventHub::Get()->removeCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));		
		m_renderCtx.reset();
		m_window.reset();
		Registry::Shutdown();
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
		onAttach();

		auto &renderThread = m_renderCtx->thread();
		renderThread.run(PIO_BIND_FN_OTHER(RenderContext::renderLoop, m_renderCtx.get()));
		// Block until the first frame has been done
		renderThread.pump();

		while(m_running)
		{
			// Wait for render thread to finish commands
			renderThread.blockUntilRenderComplete();
			renderThread.nextFrame();
			m_renderCtx->swapQueues();
			// Start rendering previous frame in render thread
			renderThread.kick();
			//---------------------------------------------------------
			
			// Calculate time interval
			Time::k_Tick.tick();

			EventBus::Get()->dispatch();
			EventHub::Get()->dispatch();

			m_sceneMgr.onUpdate(m_renderCtx);

			Time::RecordTime();
		}
		// Wait for render thread to complete the last frame
		// After this call, render thread is waiting for kick
		renderThread.blockUntilRenderComplete();
		onDetach();
	}
}