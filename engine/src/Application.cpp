#include "Application.h"

#include "core/EventBus.h"
#include "core/utils/Profiler.h"
#include "scene/Camera.h"

#include "gfx/struct/MaterialLibrary.h"
#include "gfx/debug/GDebugger.h"

#include "asset/AssetsManager.h"

#include "window/event/AppEvent.h"

#include "scene/Scene.h"
#include "scene/Registry.h"

#include "physics/PhysicsSystem.h"

#include "ui/layer/SceneHierarchyLayer.h"
#include "ui/layer/RuntimeLayer.h"
#include "ui/layer/EditorLayer.h"
#include "ui/layer/MotionControlLayer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Application"

namespace pio
{
	Application *Application::s_app = nullptr;

	Application::Application()
	{
		// Only one Application in whole program.
		s_app = this;

		LogSystem::Initialize();

		WindowProps prop{ "Pionner", 1280, 720 };
		m_window = Window::create(prop);
		m_window->setEventCallback(PIO_BIND_EVT_FN(Application::onEvent));

		m_graphics = GraphicsContext::create(m_window->getNativeWindow());

		EventBus::Init();
		Renderer::Init();
		AssetsManager::Init();
		PhysicsSystem::Init();
		Renderer::SetRenderListener(this);
	}

	Application::~Application()
	{
		EventBus::Shutdown();
		Renderer::Shutdown();
		Registry::Shutdown();
		PhysicsSystem::Shutdown();

		m_graphics.reset(nullptr);
		m_window.reset(nullptr);

		LogSystem::Shutdown();
	}

	void Application::onEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<WindowCloseEvent>(PIO_BIND_EVT_FN(Application::onWindowClose));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		dispatcher.dispatch<WindowResizeEvent>(PIO_BIND_EVT_FN(Application::onWindowResize));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		m_layerManager.onEvent(event);
	}

	bool Application::onWindowClose(Event &event)
	{
		LOGD("window is closed");
		m_running = false;
		return true;
	}

	bool Application::onWindowResize(Event &event)
	{
		return m_layerManager.onWindowResize(event);
	}

	void Application::run()
	{
		m_renderThread.run();
		// pump() will block until the first frame has been done
		m_renderThread.pump();

		onInit();

		Timestep step{};
		Timestep frameTime{};
		while (m_running)
		{
			//------- Wait for render thread to finish commands -------
			// Wait for render thread to finish renderering
			m_renderThread.blockUntilRenderComplete();
			m_renderThread.nextFrame();
			// Start rendering previous frame in render thread
			m_renderThread.kick();
			//---------------------------------------------------------

			EventBus::Get()->dispatch();

			{
				uint64_t start{ PROFILER_TIME };
				//BUG FIX: Guarantee event queue thread safety,
				//         because render thread might use that queue.
				std::lock_guard<std::mutex> lk{ EventMutex };
				m_window->pollEvents();
				PROFILERD_DURATION(start, "PollEvents");
			}

			//----------- Fill in the pending render command -----------
			step.tick();

			Renderer::SubmitRC([=]() mutable { Renderer::BeginFrame(); });
			m_layerManager.onUpdate(step);

			Renderer::SubmitRC([=]() mutable { Renderer::BeginUI(); });
			m_layerManager.onUpdateUI(step);
			Renderer::SubmitRC([=]() mutable { Renderer::EndUI(); });

			Renderer::SubmitRC([=]() mutable { Renderer::EndFrame(); });
			//----------------------------------------------------------

			Renderer::SubmitRC([&]() { m_graphics->swapBuffer(); });

			frameTime.tick();
			Renderer::GetConfig().FPS = m_frameRec.avgFps((uint64_t)frameTime);
			Renderer::GetConfig().FrameTime = (uint64_t)frameTime;
		}

		// NOTE: AssetsManager is destroied in render thread,
		//       because some assets rely on rendering context
		m_renderThread.terminate();

		onQuit();
	}

	void Application::onInit()
	{
		MaterialLibrary::Init();

		// RuntimeLayer should be added at the first place
		auto runtimeLayer = CreateRef<RuntimeLayer>(WindowLayoutParams(0.f, 0.f, 0.6f, 1.f));
		m_layerManager.pushLayer(runtimeLayer);

		auto sceneHierarchy = CreateRef<SceneHierarchyLayer>(WindowLayoutParams(0.6f, 0.f, 0.8f, 1.f));
		m_layerManager.pushLayer(sceneHierarchy);

		auto editorLayer = CreateRef<EditorLayer>(WindowLayoutParams(0.8f, 0.f, 1.f, 1.f));
		m_layerManager.pushOverlay(editorLayer);

		// MotionControlLayer's layout param should be the same as RuntimeLayer to make event processing correct
		auto ctlLayer = CreateRef<MotionControlLayer>(runtimeLayer->getLayoutParam());
		m_layerManager.pushOverlay(ctlLayer);
	}

	void Application::onQuit()
	{
	}

	void Application::onRenderInit()
	{
		// Initialize in render thread
		m_graphics->init();
		m_window->setVSync(true);
		GDebugger::Init();
	}

	void Application::onRenderDestroy()
	{
		m_layerManager.onRenderDestroy();

		Scene::Main.reset();
		Scene::Root.reset();
		Camera::Main.reset();

		m_layerManager.popAll();

		MaterialLibrary::Shutdown();

		GDebugger::Shutdown();

		// Destroy in render thread
		AssetsManager::Shutdown();
	}
}