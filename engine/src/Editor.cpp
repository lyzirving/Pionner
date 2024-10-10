#include "Editor.h"
#include "Editor.h"

#include "window/Window.h"

#include "event/AppEvent.h"
#include "event/EventBus.h"
#include "event/EventHub.h"

#include "asset/AssetMgr.h"

#include "scene/SceneMgr.h"
#include "scene/Components.h"
#include "scene/Factory.h"
#include "scene/node/CameraNode.h"
#include "scene/node/MeshNode.h"
#include "scene/node/LightNode.h"
#include "scene/node/SpriteNode.h"

#include "ui/LayerMgr.h"
#include "ui/RuntimeLayer.h"

#include "gfx/renderer/RenderContext.h"
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
		EventHub::Get()->registerCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));
		AssetMgr::Init();

		m_window = Window::create(WindowProps("Pionner", 1400, 720, RenderBackend_OpenGL));
		m_context = CreateRef<RenderContext>(RenderBackend_OpenGL, m_window);		
	}

	Editor::~Editor()
	{
		EventHub::Shutdown();
		EventBus::Shutdown();
		LogSystem::Shutdown();
	}

	void Editor::onAttach()
	{		 		
		m_pipeline = CreateRef<RenderPipeline>();
		m_pipeline->onAttach(m_context);			
	}

	void Editor::onDetach()
	{		
		LOGD("begin to destroy resource");		
		m_sceneMgr->onDetach();
		m_layerMgr->popAll();	
		AssetMgr::Shutdown();
		m_pipeline->onDetach(m_context);

		m_context->thread().terminate();
		LOGD("wake up from render thread");

		EventHub::Get()->removeCallback(EventHubCb(this, (EventHubCbFun)&Editor::onEvent));		
		m_pipeline.reset();
		m_context.reset();
		m_window.reset();
	}

	void Editor::onEvent(Ref<Event> &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<WindowCloseEvent>(PIO_BIND_FN_SELF(Editor::onWindowClose, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		m_layerMgr->onEvent(event);
	}

	bool Editor::onWindowClose(Ref<WindowCloseEvent> &event)
	{
		LOGD("window is closed");
		m_running = false;
		return true;
	}

	void Editor::run()
	{			
		auto &renderThread = m_context->thread();
		renderThread.run(PIO_BIND_FN_OTHER(RenderContext::renderLoop, m_context.get()));
		// Block until the first frame has been done
		renderThread.pump();

		onAttach();

		onPrepare();

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

			m_sceneMgr->onUpdate(m_context, m_pipeline, m_layerMgr);

			Time::RecordTime();
		}
		// Wait for render thread to complete the last frame
		// After this call, render thread is waiting for kick
		renderThread.blockUntilRenderComplete();
		onDetach();
	}

	void pio::Editor::onPrepare()
	{
		m_layerMgr = CreateRef<LayerMgr>();
		m_sceneMgr = CreateRef<SceneMgr>();

		auto scene = CreateRef<Scene>();
		auto* camNode = scene->addNode<CameraNode>(m_context, "MainCamera")->as<CameraNode>();
		camNode->setPosition(glm::vec3(0.f, 7.f, 10.f));
		camNode->setRotation(glm::vec3(-35.f, 0.f, 0.f));
		
		auto* dirLightNode = scene->addNode<DirectionalLightNode>(m_context, "DirectionalLight")->as<DirectionalLightNode>();
		dirLightNode->setPosition(glm::vec3(-4.f, 4.f, 4.f));
		dirLightNode->setRotation(glm::vec3(-30.f, -45.f, 0.f));

		scene->addNode<CubeNode>(m_context, "Cube");

		auto* bottomPlane = scene->addNode<PlaneNode>(m_context, "BottomPlane")->as<PlaneNode>();
		bottomPlane->setWidth(9.f);
		bottomPlane->setHeight(6.f);

		auto* backPlane = scene->addNode<PlaneNode>(m_context, "BackPlane")->as<PlaneNode>();
		backPlane->setWidth(9.f);
		backPlane->setHeight(9.f);
		backPlane->setRotation(glm::vec3(90.f, 0.f, 0.f));
		backPlane->setPosition(glm::vec3(0.f, 0.f, -2.5f));

		auto* rightPlane = scene->addNode<PlaneNode>(m_context, "RightPlane")->as<PlaneNode>();
		rightPlane->setWidth(12.f);
		rightPlane->setHeight(9.f);
		rightPlane->setRotation(glm::vec3(90.f, -90.f, 0.f));
		rightPlane->setPosition(glm::vec3(2.5f, 0.f, 0.f));

		m_sceneMgr->add(scene, true);

		auto runtimeLayer = CreateRef<RuntimeLayer>(LayoutParams(m_window->width(), m_window->height(), 0.f, 0.f, 1.f, 1.f));
		m_layerMgr->pushLayer(m_context, runtimeLayer);
	}
}