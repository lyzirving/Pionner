#include "Editor.h"
#include "GlobalSettings.h"

#include "window/Window.h"

#include "asset/AssetMgr.h"
#include "asset/mesh/StaticMesh.h"
#include "asset/mesh/SkinnedMesh.h"

#include "event/AppEvent.h"
#include "event/EventMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/pipeline/RenderPipeline.h"

#include "scene/SceneMgr.h"
#include "scene/node/SphereNode.h"
#include "scene/node/PlaneNode.h"
#include "scene/node/CameraNode.h"
#include "scene/node/DirectionalLitNode.h"
#include "scene/node/PointLitNode.h"
#include "scene/node/SkinnedMeshNode.h"

#include "animation/AnimationSystem.h"

#include "ui/UiDef.h"
#include "ui/WidgetMgr.h"
#include "ui/panel/ScenePanel.h"
#include "ui/panel/OutlinePanel.h"
#include "ui/panel/DetailPanel.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Editor"

namespace pio
{
	Editor* Editor::k_Editor = nullptr;

	Editor* Editor::Get() { return k_Editor; }

	Editor::Editor() : EventSocket()
	{
		k_Editor = this;

		Time::Tick();
		LogSystem::Init();
		Path::Init();
		EventMgr::Get()->ConnectWin(EventCallback(this, (SLOTFUNCTION)&Editor::OnEvent));
		TaskGraph::Init();

		m_Window = Window::Create(WindowProps("Pionner", 1400, 720, RenderBackend_OpenGL));
		m_Context = CreateRef<RenderContext>(RenderBackend_OpenGL, m_Window);
	}

	Editor::~Editor()
	{
		EventMgr::Shutdown();
		Path::Shutdown();
		LogSystem::Shutdown();
	}

	void Editor::OnAttach()
	{
		// Take care of the order of each step

		// step 1: create RenderPipeline
		m_Pipeline = CreateRef<RenderPipeline>(m_Context);
		m_Pipeline->OnAttach();

		// step 2: create SceneMgr and Scene
		m_SceneMgr = CreateRef<SceneMgr>(m_Context);
		auto scene = m_SceneMgr->CreateScene();
		m_SceneMgr->PushBack(scene);
		m_SceneMgr->OnActive();

		AnimationSystem::Init();

		// step 3: add node into scene
		auto camera = scene->CreateNode<CameraNode>("MainCamera");
		camera->SetPosition(glm::vec3(0.f, 5.f, 10.5f));
		camera->SetRotation(glm::vec3(-20.f, 0.f, 0.f));
		camera->SetAspect(GlobalSettings::AspectRatio());
		scene->Insert(camera);

		auto mainLit = scene->CreateNode<DirectionalLitNode>("DirectionalLight");
		mainLit->SetRotation(glm::vec3(-35.f, 0.f, 40.f));
		mainLit->SetPosition(glm::vec3(-3.f, 4.f, 0.f));
		mainLit->SetIntensity(3.3f);
		scene->Insert(mainLit);

		auto ptLit = scene->CreateNode<PointLitNode>("PointLit");
		ptLit->SetPosition(glm::vec3(1.5, 2.5f, 0.f));
		ptLit->SetRadius(3.5f);
		scene->Insert(ptLit);

		auto plane = scene->CreateNode<PlaneNode>("Plane");
		plane->SetScale(glm::vec3(8.f, 1.f, 8.f));
		scene->Insert(plane);

		ImportParams params;
		params.SetFmt(AssetFormat_Mesh)
			.SetPath(std::string(Path::MeshRoot()).append(Path::FILE_SEPARATOR).append("werewolf"/*"car_audi"*/))
			.SetName("scene")
			.SetSuffix("gltf")
			.SetContext(m_Context);
		AssetMgr::Get()->LoadAssetAsync(std::move(params), [weakScene = WeakRef<Scene>(scene)](const Ref<TaskRet>& ret)
		{
			auto scene = weakScene.lock();
			auto* loadRet = ret->As<LoadRet>();
			if(!scene || !loadRet)
				return;

			std::string name("Model");
			if(loadRet->m_Asset->Is<SkinnedMesh>())
			{
				auto node = scene->CreateNode<SkinnedMeshNode>(name);
				node->SetMesh(RefCast<Asset, SkinnedMesh>(loadRet->m_Asset));
				node->SetAnimationClip(0);
				node->SetLoop(true);
				node->StartAnimation();
				scene->Insert(node);
			}
			else if(loadRet->m_Asset->Is<StaticMesh>())
			{
				auto node = scene->CreateNode<StaticMeshNode>(name);
				node->SetMesh(RefCast<Asset, StaticMesh>(loadRet->m_Asset));
				scene->Insert(node);
			}
			else
			{
				LOGE("err! invalid asset type");
			}
		});

		// step 4: create Ui's WidgetMgr
		m_WidgetMgr = CreateRef<WidgetMgr>(m_Context);
		m_WidgetMgr->OnAttach(scene);

		//step 5: add UI widget
		auto outlinePanel = CreateRef<OutlinePanel>();
		outlinePanel->SetLayoutParam(LayoutParams(0.6f, 0.f, 0.75f, 1.f));
		outlinePanel->SetDrawOrder(0);

		auto detailPanel = CreateRef<DetailPanel>();
		detailPanel->SetLayoutParam(LayoutParams(0.75f, 0.f, 1.f, 1.f));
		detailPanel->SetDrawOrder(1);

		auto scenePanel = CreateRef<ScenePanel>();
		scenePanel->SetLayoutParam(LayoutParams(0.f, 0.f, 0.6f, 0.7f));
		scenePanel->EnableTitle(false);
		scenePanel->EnableDocking(false);
		scenePanel->EnableMove(false);
		scenePanel->SetDrawOrder(2);

		m_WidgetMgr->AddWidget(outlinePanel);
		m_WidgetMgr->AddWidget(detailPanel);
		m_WidgetMgr->AddWidget(scenePanel);
	}

	void Editor::OnDetach()
	{
		LOGD("begin to destroy resource");
		AnimationSystem::Shutdown();
		m_WidgetMgr->OnDetach();
		m_SceneMgr->OnDetach();
		m_SceneMgr->OnExit();
		TaskGraph::Shutdown();
		m_Pipeline->OnDetach();

		m_Context->Thread().Terminate();
		LOGD("wake up from render thread");

		EventMgr::Get()->RemoveWin(EventCallback(this, (SLOTFUNCTION)&Editor::OnEvent));
		m_WidgetMgr.reset();
		m_SceneMgr.reset();
		m_Pipeline.reset();
		m_Context.reset();
		m_Window.reset();
	}

	void Editor::OnEvent(const Ref<Event>& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(PIO_BIND_FN_SELF(Editor::OnWindowClose, std::placeholders::_1));
		PIO_CHECK_EVT_HANDLE_AND_RETURN(event);

		m_WidgetMgr->DispatchEvent(event);
	}

	bool Editor::OnWindowClose(const Ref<WindowCloseEvent>& event)
	{
		LOGD("window is closed");
		m_Running = false;
		return true;
	}

	void Editor::Run()
	{
		auto& renderThread = m_Context->Thread();
		renderThread.Run(PIO_BIND_FN_OTHER(RenderContext::RenderLoop, m_Context.get()));
		// Block until the first frame has been done
		renderThread.Pump();

		OnAttach();

		while(m_Running)
		{
			// Wait for render thread to finish commands
			renderThread.BlockUntilRenderComplete();
			renderThread.NextFrame();
			m_Context->SwapQueues();
			// Start rendering previous frame in render thread
			renderThread.Kick();
			//---------------------------------------------------------

			Tick();
		}
		// Wait for render thread to complete the last frame
		// After this call, render thread is waiting for kick
		renderThread.BlockUntilRenderComplete();
		OnDetach();
	}

	void pio::Editor::Tick()
	{
		Time::Tick();

		EventMgr::Get()->Dispatch();

		m_SceneMgr->Tick(m_Context, m_Pipeline);
		m_WidgetMgr->Tick(m_Context);
	}
}