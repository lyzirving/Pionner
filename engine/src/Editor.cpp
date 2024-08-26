#include "Editor.h"

#include "window/Window.h"

#include "event/AppEvent.h"
#include "event/EventBus.h"
#include "event/EventHub.h"

#include "asset/AssetMgr.h"

#include "scene/Components.h"
#include "scene/Factory.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/UniformData.h"
#include "gfx/pipeline/RenderPipeline.h"

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
		AssetMgr::Init();
		UniformDataPool::Init();
		EventHub::Get()->registerCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));

		m_window = Window::create(WindowProps("Pionner", 1400, 720, RenderBackend_OpenGL));
		m_context = CreateRef<RenderContext>(RenderBackend_OpenGL, m_window);
		m_pipeline = CreateRef<RenderPipeline>();

		auto scene = CreateRef<Scene>();
		{
			//Default entities
			Factory::MakeCamera(scene, "MainCamera", 0);
			Factory::MakePlane(scene, "Plane");
		}
		m_sceneMgr.add(scene, true);
	}

	void Editor::onDetach()
	{		
		LOGD("begin to destroy resource");
		auto& renderThread = m_context->thread();

		m_sceneMgr.removeAll();
		AssetMgr::Shutdown();

		renderThread.terminate();
		LOGD("wake up from render thread");

		EventHub::Get()->removeCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));
		UniformDataPool::Shutdown();
		m_pipeline.reset();
		m_context.reset();
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
		onAttach();

		auto &renderThread = m_context->thread();
		renderThread.run(PIO_BIND_FN_OTHER(RenderContext::renderLoop, m_context.get()));
		// Block until the first frame has been done
		renderThread.pump();

		while(m_running)
		{
			// Wait for render thread to finish commands
			renderThread.blockUntilRenderComplete();
			renderThread.nextFrame();
			m_context->swapQueues();
			// Start rendering previous frame in render thread
			renderThread.kick();
			//---------------------------------------------------------
			
			// Calculate time interval
			Time::k_Tick.tick();

			EventBus::Get()->dispatch();
			EventHub::Get()->dispatch();

			m_sceneMgr.onUpdate(m_context, m_pipeline);

			Time::RecordTime();
		}
		// Wait for render thread to complete the last frame
		// After this call, render thread is waiting for kick
		renderThread.blockUntilRenderComplete();
		onDetach();
	}
}