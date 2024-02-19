#include "C3DControlLayer.h"

#include "Application.h"

#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Geometry.h"
#include "gfx/debug/GDebugger.h"

#include "physics/PhysicsScene.h"
#include "physics/PhysicsActor.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "asset/AssetsManager.h"
#include "window/event/MouseEvent.h"
#include "ui/struct/Ui3D.h"

#include "core/math/Ray.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "C3DControlLayer"

namespace pio
{
	#define CTL_CAM_RADIUS (2.f)
	#define CTL_TRANSLATION_RATIO (7.f)

	C3DControlLayer::C3DControlLayer(const LayoutParams &param)
		: Layer(param, "C3DControlLayer")
	{
	}

	void C3DControlLayer::onAttach()
	{
		m_circleLayoutParam.Percentage = LayoutPercentage(m_layoutParam.Percentage.Right - 0.1f, 
														  m_layoutParam.Percentage.Top + 0.015f,
														  m_layoutParam.Percentage.Right,
														  m_layoutParam.Percentage.Top + 0.125f);

		m_mainCameraEnt = s_registry->mainCameraEnt();
		m_sceneEnt = s_registry->mainSceneEnt();
		m_world = CreateRef<PhysicsScene>("Control Panel");

		m_visionCamUD.obtainBlock();
		m_visionUBSet = UniformBufferSet::Create();
		m_visionUBSet->create(m_visionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);

		m_selectionCamUD.obtainBlock();
		m_selectionUBSet = UniformBufferSet::Create();
		m_selectionUBSet->create(m_selectionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);

		UiCoordinate3DBuilder visionBuilder;
		visionBuilder.Shape = CoordinateShape::Cylinder;
		m_visionCoord = CreateRef<UiCoordinate3D>(visionBuilder);

		UiCoordinate3DBuilder selectBuilder;
		selectBuilder.Shape = CoordinateShape::Arrow;
		m_selectCoord = CreateRef<UiCoordinate3D>(selectBuilder);

		Ref<PhysicsActor> xActor = m_world->createActor<C3dUIComponent>(m_selectCoord->XAxisEnt, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> yActor = m_world->createActor<C3dUIComponent>(m_selectCoord->YAxisEnt, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> zActor = m_world->createActor<C3dUIComponent>(m_selectCoord->ZAxisEnt, RigidBodyComponent::Type::Dynamic);

		xActor->setActorTransform(glm::vec3(m_selectCoord->Builder.ArrowInfo.Offset, 0.f, 0.f),
								  glm::angleAxis(glm::radians(-90.f), AXIS_Z));
		yActor->setActorTransform(glm::vec3(0.f, m_selectCoord->Builder.ArrowInfo.Offset, 0.f),
								  quaternion::IDENTITY);
		zActor->setActorTransform(glm::vec3(0.f, 0.f, m_selectCoord->Builder.ArrowInfo.Offset),
								  glm::angleAxis(glm::radians(90.f), AXIS_X));

		onWindowSizeChange(Application::MainWindow()->getWidth(),
						   Application::MainWindow()->getHeight());
	}

	void C3DControlLayer::onDetach()
	{
		m_visionUBSet.reset();
		m_selectionUBSet.reset();
		m_visionCoord.reset();
		m_selectCoord.reset();

		m_world.reset();
	}

	bool C3DControlLayer::onEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<MouseButtonPressedEvent>(PIO_BIND_EVT_FN(C3DControlLayer::onMouseButtonPressed));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseMovedEvent>(PIO_BIND_EVT_FN(C3DControlLayer::onMouseMoved));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseButtonReleasedEvent>(PIO_BIND_EVT_FN(C3DControlLayer::onMouseButtonReleased));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseScrolledEvent>(PIO_BIND_EVT_FN(C3DControlLayer::onMouseScrolled));
		if (event.Handled) return true;

		return false;
	}

	void C3DControlLayer::onUpdate(const Timestep &ts)
	{	
		// Circle background
		if (m_drawCircle)
		{
			LayoutRect &c = m_circleLayoutParam.Position;
			Renderer::SubmitRC([c]() mutable
			{
				ImDrawList *drawList = ImGui::GetForegroundDrawList();
				drawList->AddCircleFilled(ImVec2(c.Left + c.width() * 0.5f, c.Top + c.height() * 0.5f), c.height() * 0.5f, ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.3f)));
			});
		}

		onDrawVisionCtl(ts);
		onDrawSelectionCtl(ts);
	}

	void C3DControlLayer::onWindowSizeChange(uint32_t width, uint32_t height)
	{
		m_layoutParam.calculate(width, height);
		m_circleLayoutParam.calculate(width, height);

		LayoutRect lp = m_circleLayoutParam.Position;
        float radius = (lp.Bottom - lp.Top) * 0.5f;
        glm::vec2 center(lp.Right - radius - 3, (lp.Top + lp.Bottom) * 0.5f);

		m_circleLayoutParam.Position.Left = center.x - radius;
		m_circleLayoutParam.Position.Right = center.x + radius;
		m_circleLayoutParam.Position.Top = center.y - radius;
		m_circleLayoutParam.Position.Bottom = center.y + radius;

		m_circleLayoutParam.Viewport.Width = m_circleLayoutParam.Position.Right - m_circleLayoutParam.Position.Left;
		m_circleLayoutParam.Viewport.Height = m_circleLayoutParam.Position.Bottom - m_circleLayoutParam.Position.Top;
		m_circleLayoutParam.Viewport.X = m_circleLayoutParam.Position.Left;
		m_circleLayoutParam.Viewport.Y = height - m_circleLayoutParam.Position.Top - m_circleLayoutParam.Viewport.Height;
	}

	bool C3DControlLayer::onMouseButtonPressed(Event &event)
	{
		m_objSelectd = false;
		glm::vec2 cursor = Application::MainWindow()->getCursorPos();
		// ------------------- Button pressed work flow --------------------
		if (m_circleLayoutParam.Position.contain((uint32_t)cursor.x, (uint32_t)cursor.y))
		{
			m_eventState.ButtonPressed = true;
			m_eventState.LastCursor = cursor;
			Application::MainWindow()->setCursorMode(CursorMode::Disabled);
			return true;
		}
		// ------------------------------------------------------------------
		// Two mutually exclusive work flow
		// ------------------- Object selection work flow -------------------
		auto &sceneComp = m_sceneEnt->getComponent<SceneComponent>();
		if (sceneComp.Selected3D)
		{
			glm::uvec2 viewportPt = UiDef::ScreenToViewport(cursor, m_layoutParam);
			Ray r = Ray::BuildFromScreen(viewportPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
			HitResult result = m_world->intersect(r);
			onIntersect(result);
			if (m_objSelectd = result.Hit)
			{
				m_eventState.LastCursor = cursor;
			}
			GDebugger::Get()->drawLine(r, 10.f, glm::vec4(0.f, 0.f, 1.f, 1.f));
		}
		// ------------------------------------------------------------------
		return m_objSelectd;
	}

	bool C3DControlLayer::onMouseMoved(Event &event)
	{
		auto *p = event.as<MouseMovedEvent>();
		// ------------------- Button pressed work flow --------------------
		if (m_eventState.ButtonPressed)
		{
			m_eventState.Cursor = glm::vec2(p->getX(), p->getY());
			m_drawCircle = true;
			glm::vec2 delta = m_eventState.Cursor - m_eventState.LastCursor;
			delta.x = Math::Clamp(delta.x, -20.f, 20.f);
			delta.y = Math::Clamp(delta.y, -20.f, 20.f);
			m_eventState.LastCursor = m_eventState.Cursor;
			auto &comp = m_mainCameraEnt->getComponent<CameraComponent>();
			comp.Camera.addPosDiff(delta.x, delta.y);
			return true;
		}
		m_drawCircle = m_circleLayoutParam.Position.contain(p->getX(), p->getY());
		// ------------------------------------------------------------------
		// ------------------- Object selection work flow -------------------
		if (m_objSelectd)
		{
			auto &sceneComp = m_sceneEnt->getComponent<SceneComponent>();
			m_eventState.Cursor = glm::vec2(p->getX(), p->getY());
			onSelectionMoved(sceneComp.Selected3D, m_hitCtlActor, m_eventState.Cursor, m_eventState.LastCursor, m_layoutParam);
			m_eventState.LastCursor = m_eventState.Cursor;
			return true;
		}
		// ------------------------------------------------------------------
		return false;
	}

	bool C3DControlLayer::onMouseButtonReleased(Event &event)
	{
		bool pressed = m_eventState.ButtonPressed;
		bool consume = pressed || m_objSelectd;
		if (pressed) { Application::MainWindow()->setCursorMode(CursorMode::Normal); }
		m_eventState.ButtonPressed = false;
		m_eventState.LastCursor = m_eventState.Cursor = glm::vec2(-1.f);
		m_objSelectd = false;
		return consume;
	}

	bool C3DControlLayer::onMouseScrolled(Event &event)
	{
		glm::vec2 cursor = Application::MainWindow()->getCursorPos();
		bool inside = m_circleLayoutParam.Position.contain((uint32_t)cursor.x, (uint32_t)cursor.y);
		if (inside)
		{
			auto *p = event.as<MouseScrolledEvent>();
			auto &comp = m_mainCameraEnt->getComponent<CameraComponent>();
			comp.Camera.addRadiusDiff(-p->getYOffset() * 0.5f/*scale factor*/);
		}
		return inside;
	}

	void C3DControlLayer::onDrawVisionCtl(const Timestep &ts)
	{
		CameraComponent &camComp = m_mainCameraEnt->getComponent<CameraComponent>();
		Camera &camera = camComp.Camera;
		LayoutViewport &vp = m_circleLayoutParam.Viewport;

		CameraUD &cameraUD = m_visionCamUD;
		Ref<UniformBufferSet> ubSet = m_visionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		SphereCoord sceneCamPos = camComp.Camera.getCameraPosSC();
		// Make ui camera's radius remain unchange
		sceneCamPos.setRadius(CTL_CAM_RADIUS);

		cameraUD.ViewMat = Camera::GetViewMat(sceneCamPos, glm::vec3(0.f));
		cameraUD.PrjMat = camera.getPrjMat();
		cameraUD.OrthoMat = camera.getOrthoMat();
		cameraUD.CameraPosition = camera.getCameraPos();
		cameraUD.FrustumFar = camera.far();
		cameraUD.serialize();

		{
			Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
			{
				Renderer::CommitViewport(Viewport{ (int32_t)vp.X, (int32_t)vp.Y, (int32_t)vp.Width, (int32_t)vp.Height });
				cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
			});
		}

		// X Axis
		{
			auto &meshComp = m_visionCoord->XAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}

		// Y Axis
		{
			auto &meshComp = m_visionCoord->YAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}

		// Z Axis
		{
			auto &meshComp = m_visionCoord->ZAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}
	}

	void C3DControlLayer::onDrawSelectionCtl(const Timestep &ts)
	{
		glm::vec3 slcPos(0.f);
		auto &sceneComp = m_sceneEnt->getComponent<SceneComponent>();
		Ref<Entity> selection = sceneComp.Selected3D;
		if (!(selection && selection->getGlobalPoseCenter(slcPos)))
		{
			return;
		}

		CameraComponent &camComp = m_mainCameraEnt->getComponent<CameraComponent>();
		Camera &camera = camComp.Camera;
		const Viewport &vp = camera.getViewport();

		CameraUD &cameraUD = m_selectionCamUD;
		Ref<UniformBufferSet> ubSet = m_selectionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		cameraUD.ViewMat = camera.getViewMat();
		cameraUD.PrjMat = camera.getPrjMat();
		cameraUD.OrthoMat = camera.getOrthoMat();
		cameraUD.CameraPosition = camera.getCameraPos();
		cameraUD.FrustumFar = camera.far();
		cameraUD.serialize();

		{
			Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
			{
				Renderer::CommitViewport(Viewport{ vp.X, vp.Y, vp.Width, vp.Height });
				cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
			});
		}

		// X Axis
		{
			Ref<PhysicsActor> actor;
			PIO_ASSERT_RETURN(m_selectCoord->XAxisEnt->getActor(actor), "onDrawSelectionCtl: fail to get X axis's actor");
			actor->setGlobalPose(slcPos);

			auto &meshComp = m_selectCoord->XAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = glm::translate(glm::mat4(1.f), slcPos);
			transform = transform * meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}

		// Y Axis
		{
			Ref<PhysicsActor> actor;
			PIO_ASSERT_RETURN(m_selectCoord->YAxisEnt->getActor(actor), "onDrawSelectionCtl: fail to get Y axis's actor");
			actor->setGlobalPose(slcPos);

			auto &meshComp = m_selectCoord->YAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = glm::translate(glm::mat4(1.f), slcPos);
			transform = transform * meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}

		// Z Axis
		{
			Ref<PhysicsActor> actor;
			PIO_ASSERT_RETURN(m_selectCoord->ZAxisEnt->getActor(actor), "onDrawSelectionCtl: fail to get Z axis's actor");
			actor->setGlobalPose(slcPos);

			auto &meshComp = m_selectCoord->ZAxisEnt->getComponent<C3dUIComponent>();

			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = glm::translate(glm::mat4(1.f), slcPos);
			transform = transform * meshSrc->getSubmeshes()[ind].Transform;

			RenderState state;
			state.Blend = Blend::Disable();
			state.Mode = RenderMode::MaterialPreview;
			state.DepthTest = DepthTest::Always();

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		}
	}

	void C3DControlLayer::onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const LayoutParams &param)
	{
		if (!ctlActor || !selection)
			return;

		Ref<Entity> ctlEnt;
		PIO_ASSERT_RETURN(ctlActor->getEnt(ctlEnt), "onSelectionMoved: entity is invalid");

		glm::uvec2 curPt = UiDef::ScreenToViewport(cursor, param);
		glm::uvec2 lastPt = UiDef::ScreenToViewport(last, param);

		Ray curRay = Ray::BuildFromScreen(curPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
		Ray lastRay = Ray::BuildFromScreen(lastPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);

		glm::vec3 dirDiff = curRay.Dir - lastRay.Dir;

		auto &uiComp = ctlEnt->getComponent<C3dUIComponent>();
		glm::vec3 diff3d(0.f);
		if (std::strcmp(uiComp.Name.data(), STR_AXIS_X) == 0)
		{
			diff3d.x = dirDiff.x;
		}
		else if (std::strcmp(uiComp.Name.data(), STR_AXIS_Y) == 0)
		{
			diff3d.y = dirDiff.y;
		}
		else if (std::strcmp(uiComp.Name.data(), STR_AXIS_Z) == 0)
		{
			diff3d.z = dirDiff.z;
		}
		else
		{
			LOGE("invalid selection");
			return;
		}
		diff3d *= CTL_TRANSLATION_RATIO;

		if (selection->hasComponent<ProxyComponent>())
		{
			auto &comp = selection->getComponent<ProxyComponent>();
			comp.Proxy->setGlobalPoseDiff(diff3d);
		}
		else
		{
			selection->setGlobalPoseDiff(diff3d);
		}		
	}

	void C3DControlLayer::onIntersect(const HitResult &result)
	{
		m_hitCtlActor = result.Hit ? result.Actor : nullptr;
	}
}