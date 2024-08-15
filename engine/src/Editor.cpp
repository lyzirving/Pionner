#include "Editor.h"

#include "window/Window.h"
#include "window/event/AppEvent.h"

#include "core/EventBus.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace pio
{
	Editor *Editor::k_Editor = nullptr;

	Editor *Editor::Get() { return k_Editor; }

	Editor::Editor()
	{
		k_Editor = this;

		LogSystem::Initialize();
		EventBus::Init();		
	}

	Editor::~Editor()
	{
		EventBus::Shutdown();
		LogSystem::Shutdown();
	}

	void Editor::onInit()
	{		
		WindowProps prop{ "Pionner", 1400, 720 };
		m_window = Window::create(prop);
		m_window->setEventCallback(PIO_BIND_FN_SELF(Editor::onEvent, std::placeholders::_1));

		m_renderCtx = CreateRef<RenderContext>(CRenderApiType_OpenGL, m_window);
	}

	void Editor::onQuit()
	{		
		m_renderCtx.reset();
		m_window.reset();
	}

	void Editor::onEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<WindowCloseEvent>(PIO_BIND_FN_SELF(Editor::onWindowClose, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		dispatcher.dispatch<WindowResizeEvent>(PIO_BIND_FN_SELF(Editor::onWindowResize, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);
	}

	bool Editor::onWindowClose(Event &event)
	{
		LOGD("window is closed");
		m_running = false;
		return true;
	}

	bool Editor::onWindowResize(Event &event)
	{
		return false;
	}

	void Editor::run()
	{
		onInit();

		auto &renderThread = m_renderCtx->thread();
		renderThread.run(PIO_BIND_FN_OTHER(RenderContext::renderLoop, m_renderCtx.get()));

		while(m_running)
		{
			EventBus::Get()->dispatch();

			m_window->pollEvents();
		}

		onQuit();
	}
}