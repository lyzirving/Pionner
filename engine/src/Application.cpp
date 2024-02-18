#include "Application.h"

#include "core/EventBus.h"

#include "gfx/struct/MaterialLibrary.h"

#include "asset/AssetsManager.h"

#include "window/event/AppEvent.h"

#include "scene/Registry.h"

#include "physics/PhysicsSystem.h"

#include "ui/layer/SceneHierarchyLayer.h"
#include "ui/layer/RuntimeLayer.h"
#include "ui/layer/EditorLayer.h"
#include "ui/layer/C3DControlLayer.h"

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
				//BUG FIX: Guarantee event queue thread safety,
				//         because render thread might use that queue.
				std::lock_guard<std::mutex> lk{ EventMutex };
				m_window->pollEvents();
			}

			//----------- Fill in the pending render command -----------
			step.tick();

			Renderer::SubmitRC([=]() mutable
			{
				Renderer::BeginFrame();
			});

			m_layerManager.onUpdate(step);

			Renderer::SubmitRC([=]() mutable
			{
				Renderer::EndFrame();
			});
			//----------------------------------------------------------

			Renderer::SubmitRC([&]()
			{
				m_graphics->swapBuffer();
			});

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
		auto runtimeLayer = CreateRef<RuntimeLayer>(LayoutParams(0.2f, 0.f, 0.8f, 1.f));
		m_layerManager.pushLayer(runtimeLayer);

		auto sceneHierarchy = CreateRef<SceneHierarchyLayer>(LayoutParams(0.f, 0.f, 0.2f, 1.f));
		m_layerManager.pushLayer(sceneHierarchy);

		auto ctlLayer = CreateRef<C3DControlLayer>(runtimeLayer->getLayoutParam());
		m_layerManager.pushOverlay(ctlLayer);

		auto editorLayer = CreateRef<EditorLayer>(LayoutParams(0.8f, 0.f, 1.f, 1.f));
		m_layerManager.pushOverlay(editorLayer);
	}

	void Application::onQuit()
	{
	}

	void Application::onRenderInit()
	{
		// Initialize in render thread
		m_graphics->init();
	}

	void Application::onRenderDestroy()
	{
		m_layerManager.onRenderDestroy();

		m_layerManager.popAll();

		MaterialLibrary::Shutdown();
		// Destroy in render thread
		AssetsManager::Shutdown();
	}
}