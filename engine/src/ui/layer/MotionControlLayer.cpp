#include "MotionControlLayer.h"

#include "Application.h"

#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/rhi/Texture.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Geometry.h"

#include "physics/PhysicsScene.h"
#include "physics/PhysicsActor.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "ui/Ui3D.h"
#include "ui/View.h"

#include "asset/AssetsManager.h"
#include "window/event/MouseEvent.h"

#include "core/math/Ray.h"

#include <imgui.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MotionControlLayer"

namespace pio
{
#define CTL_CAM_RADIUS (2.f)
#define CTL_TRANSLATION_RATIO (7.f)
#define CTL_ROTATE_RATIO (100.f)

	const std::string MotionControlLayer::ICON_ID_NORMAL[MotionCtl_Num] = { AssetsManager::IconAbsPath("move_normal", AssetFmt::PNG),
																			 AssetsManager::IconAbsPath("rotate_normal", AssetFmt::PNG),
																			 AssetsManager::IconAbsPath("scale_normal", AssetFmt::PNG) };

	const std::string MotionControlLayer::ICON_ID_SELECT[MotionCtl_Num] = { AssetsManager::IconAbsPath("move_selected", AssetFmt::PNG),
																			AssetsManager::IconAbsPath("rotate_selected", AssetFmt::PNG),
																			AssetsManager::IconAbsPath("scale_selected", AssetFmt::PNG) };

	MotionControlLayer::MotionControlLayer(const WindowLayoutParams &param)
		: Layer(param, "MotionControlLayer")
	{
	}

	void MotionControlLayer::onAttach()
	{
		m_circleLayoutParam.Percentage = LayoutPercentage(m_layoutParam.Percentage.Right - 0.1f,
														  m_layoutParam.Percentage.Top + 0.015f,
														  m_layoutParam.Percentage.Right,
														  m_layoutParam.Percentage.Top + 0.125f);

		m_mainCameraEnt = s_registry->mainCameraEnt();
		m_sceneEnt = s_registry->mainSceneEnt();
		
		m_motionCtlPhysx[MotionCtl_Move] = CreateRef<PhysicsScene>("Move Ctl Physx");
		m_motionCtlPhysx[MotionCtl_Rotation] = CreateRef<PhysicsScene>("Rotate Ctl Physx");

		m_views[MotionCtl_Move] = CreateRef<View>("Move Ctl View");
		m_views[MotionCtl_Move]->setStatus(ViewCtlStatus_Selected);
		m_views[MotionCtl_Move]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_SELECT[MotionCtl_Move]));

		m_views[MotionCtl_Rotation] = CreateRef<View>("Rotate Ctl View");
		m_views[MotionCtl_Rotation]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_NORMAL[MotionCtl_Rotation]));

		m_controller.SelectedView = m_views[MotionCtl_Move];
		m_controller.Mode = MotionCtl_Move;

		m_visionCamUD.obtainBlock();
		m_visionUBSet = UniformBufferSet::Create();
		m_visionUBSet->create(m_visionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);

		m_motionCamUD.obtainBlock();
		m_motionUBSet = UniformBufferSet::Create();
		m_motionUBSet->create(m_motionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);

		UiCoordinate3DBuilder visionBuilder;
		visionBuilder.Shape = CoordinateShape::Cylinder;
		m_visionCoord = CreateRef<UiCoordinate3D>(visionBuilder);

		UiCoordinate3DBuilder selectBuilder;
		selectBuilder.Shape = CoordinateShape::Arrow;
		m_selectCoord = CreateRef<UiCoordinate3D>(selectBuilder);

		Ref<PhysicsActor> scActorX = m_motionCtlPhysx[MotionCtl_Move]->createActor<C3dUIComponent>(m_selectCoord->XAxisEnt, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> scActorY = m_motionCtlPhysx[MotionCtl_Move]->createActor<C3dUIComponent>(m_selectCoord->YAxisEnt, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> scActorZ = m_motionCtlPhysx[MotionCtl_Move]->createActor<C3dUIComponent>(m_selectCoord->ZAxisEnt, RigidBodyComponent::Type::Dynamic);

		scActorX->setActorTransform(glm::vec3(m_selectCoord->Builder.ArrowInfo.Offset, 0.f, 0.f), glm::angleAxis(glm::radians(-90.f), AXIS_Z));
		scActorY->setActorTransform(glm::vec3(0.f, m_selectCoord->Builder.ArrowInfo.Offset, 0.f), quaternion::IDENTITY);
		scActorZ->setActorTransform(glm::vec3(0.f, 0.f, m_selectCoord->Builder.ArrowInfo.Offset), glm::angleAxis(glm::radians(90.f), AXIS_X));

		m_rotateCtl = CreateRef<UiRotationCtl>();
		Ref<PhysicsActor> torusActorX = m_motionCtlPhysx[MotionCtl_Rotation]->createActor<C3dUIComponent>(m_rotateCtl->XTorus, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> torusActorY = m_motionCtlPhysx[MotionCtl_Rotation]->createActor<C3dUIComponent>(m_rotateCtl->YTorus, RigidBodyComponent::Type::Dynamic);
		Ref<PhysicsActor> torusActorZ = m_motionCtlPhysx[MotionCtl_Rotation]->createActor<C3dUIComponent>(m_rotateCtl->ZTorus, RigidBodyComponent::Type::Dynamic);
		torusActorX->setActorTransform(glm::vec3(0.f), glm::angleAxis(glm::radians(90.f), AXIS_Y));
		torusActorY->setActorTransform(glm::vec3(0.f), glm::angleAxis(glm::radians(-90.f), AXIS_X));
		torusActorZ->setActorTransform(glm::vec3(0.f), quaternion::IDENTITY);

		m_uiDistantLight = CreateRef<UiDistantLight>(0.5f, 2.f, glm::vec4(0.964f, 0.953f, 0.051f, 1.f));

		onWindowSizeChange(Application::MainWindow()->getWidth(),
						   Application::MainWindow()->getHeight());
	}

	void MotionControlLayer::onDetach()
	{
		m_visionUBSet.reset();
		m_motionUBSet.reset();
		m_visionCoord.reset();
		m_selectCoord.reset();
		m_rotateCtl.reset();
		m_uiDistantLight.reset();

		for (uint32_t i = 0; i < MotionCtl_Num; i++)
			m_motionCtlPhysx[i].reset();
	}

	bool MotionControlLayer::onEvent(Event &event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<MouseButtonPressedEvent>(PIO_BIND_EVT_FN(MotionControlLayer::onMouseButtonPressed));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseMovedEvent>(PIO_BIND_EVT_FN(MotionControlLayer::onMouseMoved));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseButtonReleasedEvent>(PIO_BIND_EVT_FN(MotionControlLayer::onMouseButtonReleased));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseScrolledEvent>(PIO_BIND_EVT_FN(MotionControlLayer::onMouseScrolled));
		if (event.Handled) return true;

		return false;
	}

	void MotionControlLayer::onUpdate(const Timestep &ts)
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
		onDrawMotionCtl(ts);
		onDrawMotionView(ts);
	}

	void MotionControlLayer::onWindowSizeChange(uint32_t width, uint32_t height)
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


		uint32_t l{ 10 }, t{ 5 }, viewWid{ 30 }, viewHeight{ 30 };
		m_views[MotionCtl_Move]->setPosition(l, t, viewWid, viewHeight);
		m_views[MotionCtl_Rotation]->setPosition(l + viewWid, t, viewWid, viewHeight);

		m_views[MotionCtl_Move]->setViewport(0, 0, m_layoutParam.Viewport.Width, m_layoutParam.Viewport.Height);
		m_views[MotionCtl_Rotation]->setViewport(0, 0, m_layoutParam.Viewport.Width, m_layoutParam.Viewport.Height);

		m_viewIconsRect = m_views[MotionCtl_Move]->getRect();
		for (uint8_t i = MotionCtl_Rotation; i < MotionCtl_Num; i++)
		{
			if (m_views[i])
				m_viewIconsRect.doUnion(m_views[i]->getRect());
		}
	}

	bool MotionControlLayer::onMouseButtonPressed(Event &event)
	{
		glm::vec2 winCursor = Application::MainWindow()->getCursorPos();
		glm::ivec2 viewportPt = UiDef::ScreenToViewport(winCursor, m_layoutParam);
		m_controller.DownTime = TimeUtil::CurrentTimeMs();
		m_controller.LastWinCursor = winCursor;

		// ------------------- Vision control pressing work flow ----------------------
		if (m_circleLayoutParam.Position.contain((uint32_t)winCursor.x, (uint32_t)winCursor.y))
		{
			m_controller.setTarget(MotionTarget_Vision);
			Application::MainWindow()->setCursorMode(CursorMode::Disabled);
			return true;
		}
		// -------------------------------------------------------------------

		// ------------------- Object3d pressing work flow --------------------
		// An entity is selected, check whether the it is interacting with the controller
		if (m_controller.SelectedObj3D)
		{
			Ray r = Ray::BuildFromScreen(viewportPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
			HitResult result = m_motionCtlPhysx[m_controller.Mode]->intersect(r);
			m_controller.CtlActor = result.Hit ? result.Actor : nullptr;
			if (m_controller.CtlActorPressed = result.Hit)
			{
				m_controller.setTarget(MotionTarget_Object3D);
			}
			return m_controller.CtlActorPressed;
		}
		// -------------------------------------------------------------------	

		// ------------------- Sprite pressing work flow ----------------------
		if (m_controller.SelectedSprite)
		{
			Ray r = Ray::BuildFromScreen(viewportPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
			HitResult result = m_motionCtlPhysx[m_controller.Mode]->intersect(r);
			m_controller.CtlActor = result.Hit ? result.Actor : nullptr;
			if (m_controller.CtlActorPressed = result.Hit)
			{
				m_controller.setTarget(MotionTarget_Sprite);
			}
			return m_controller.CtlActorPressed;
		}
		// -------------------------------------------------------------------	

		return false;
	}

	bool MotionControlLayer::onMouseMoved(Event &event)
	{
		auto *p = event.as<MouseMovedEvent>();
		glm::vec2 windowCursor(p->getX(), p->getY());

		// ------------------- Vision control pressing work flow ----------------------		
		if (m_controller.bTarget(MotionTarget_Vision))
		{
			m_drawCircle = true;
			glm::vec2 delta = windowCursor - m_controller.LastWinCursor;
			delta.x = Math::Clamp(delta.x, -20.f, 20.f);
			delta.y = Math::Clamp(delta.y, -20.f, 20.f);
			m_controller.LastWinCursor = windowCursor;
			auto &comp = m_mainCameraEnt->getComponent<CameraComponent>();
			comp.Camera.addPosDiff(delta.x, delta.y);
			return true;
		}
		m_drawCircle = m_circleLayoutParam.Position.contain(p->getX(), p->getY());
		// -------------------------------------------------------------------

		// ------------------- Object3d pressing work flow -------------------
		if (m_controller.bTarget(MotionTarget_Object3D))
		{
			onSelectionMoved(m_controller.SelectedObj3D, m_controller.CtlActor, windowCursor, m_controller.LastWinCursor, m_layoutParam);
			m_controller.LastWinCursor = windowCursor;
			return true;
		}

		// ------------------- Sprite pressing work flow ----------------------
		if (m_controller.bTarget(MotionTarget_Sprite))
		{
			onSelectionMoved(m_controller.SelectedSprite, m_controller.CtlActor, windowCursor, m_controller.LastWinCursor, m_layoutParam);
			m_controller.LastWinCursor = windowCursor;
			return true;
		}
		// ------------------------------------------------------------------	
		return false;
	}

	bool MotionControlLayer::onMouseButtonReleased(Event &event)
	{
		bool bUsing = !m_controller.bTarget(MotionTarget_None);
		if (!bUsing && MotionController::IsClick(TimeUtil::CurrentTimeMs(), m_controller.DownTime))
		{
			bUsing = onHandleClick(Application::MainWindow()->getCursorPos());
		}

		if (m_controller.bTarget(MotionTarget_Vision))
		{
			Application::MainWindow()->setCursorMode(CursorMode::Normal);
		}

		m_controller.LastWinCursor = glm::vec2(-1.f);
		m_controller.CtlActor = nullptr;
		m_controller.CtlActorPressed = false;
		m_controller.SelectedAxis = MotionCtlAxis_Num;
		m_controller.setTarget(MotionTarget_None);

		return bUsing;
	}

	bool MotionControlLayer::onMouseScrolled(Event &event)
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

	void MotionControlLayer::onDrawVisionCtl(const Timestep &ts)
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

		Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
		{
			Renderer::CommitViewport(Viewport{ (int32_t)vp.X, (int32_t)vp.Y, (int32_t)vp.Width, (int32_t)vp.Height });
			cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
		});

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

	void MotionControlLayer::onDrawMotionCtl(const Timestep &ts)
	{
		switch (m_controller.Mode)
		{
			case MotionCtl_Move:
			{
				onDrawMoveMode(ts);
				break;
			}
			case MotionCtl_Rotation:
			{
				onDrawRotationMode(ts);
				break;
			}
			default:
				break;
		}
	}

	void MotionControlLayer::onDrawMotionView(const Timestep &ts)
	{
		CameraComponent &camComp = m_mainCameraEnt->getComponent<CameraComponent>();
		Camera &camera = camComp.Camera;
		const Viewport &vp = camera.getViewport();

		Renderer::SubmitRC([vp]() mutable
		{
			Renderer::CommitViewport(Viewport{ vp.X, vp.Y, vp.Width, vp.Height });
		});

		RenderState s;
		s.Blend = Blend::Disable();
		s.DepthTest = DepthTest::Always();
		s.Cull = CullFace::Common();
		s.Stencil.Enable = false;

		for (uint8_t i = 0; i < MotionCtl_Num; i++)
		{
			if (!m_views[i]) continue;

			Ref<View> v = m_views[i];
			if (v->needUpdate())
				Renderer::SubmitTask([v]() mutable { v->upload(); });

			Renderer::SubmitRC([v, s]() mutable
			{
				Renderer::RenderTextureQuad2D(v->getMesh(), v->getTexture(), s);
			});
		}
	}

	void MotionControlLayer::onDrawMoveMode(const Timestep &ts)
	{
		glm::vec3 slcPos(0.f);
		if (m_controller.SelectedSprite)
		{
			if (m_controller.SelectedSprite->hasComponent<DirectionalLightComponent>())
			{
				auto &transComp = m_controller.SelectedSprite->getComponent<TransformComponent>();
				slcPos = transComp.Transform.Position;
				onDrawMoveCtl(slcPos);
				onDrawUIDistantLight(slcPos);
				return;
			}
		}

		if (m_controller.SelectedObj3D && m_controller.SelectedObj3D->getGlobalPoseCenter(slcPos))
		{
			onDrawMoveCtl(slcPos);
			return;
		}
	}

	void MotionControlLayer::onDrawRotationMode(const Timestep &ts)
	{
		glm::vec3 slcPos(0.f);
		if (m_controller.SelectedSprite)
		{
			if (m_controller.SelectedSprite->hasComponent<DirectionalLightComponent>())
			{
				auto &transComp = m_controller.SelectedSprite->getComponent<TransformComponent>();
				slcPos = transComp.Transform.Position;		
				onDrawRotationCtl(slcPos);
				onDrawUIDistantLight(slcPos);
				return;
			}
		}

		if (m_controller.SelectedObj3D && m_controller.SelectedObj3D->getGlobalPoseCenter(slcPos))
		{
			onDrawRotationCtl(slcPos);
			return;
		}
	}

	void MotionControlLayer::onDrawMoveCtl(const glm::vec3 pos)
	{
		CameraComponent &camComp = m_mainCameraEnt->getComponent<CameraComponent>();
		Camera &camera = camComp.Camera;
		const Viewport &vp = camera.getViewport();

		CameraUD &cameraUD = m_motionCamUD;
		Ref<UniformBufferSet> ubSet = m_motionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		cameraUD.ViewMat = camera.getViewMat();
		cameraUD.PrjMat = camera.getPrjMat();
		cameraUD.OrthoMat = camera.getOrthoMat();
		cameraUD.CameraPosition = camera.getCameraPos();
		cameraUD.FrustumFar = camera.far();
		cameraUD.serialize();

		Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
		{
			Renderer::CommitViewport(Viewport{ vp.X, vp.Y, vp.Width, vp.Height });
			cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
		});

		auto drawFunc = [ubSet](C3dUIComponent &_meshComp, const glm::vec3 _pos) mutable
		{
			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(_meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(_meshComp.SourceHandle);
			uint32_t ind = _meshComp.SubmeshIndex;
			RenderState state = _meshComp.State;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = glm::translate(glm::mat4(1.f), _pos);
			transform = transform * meshSrc->getSubmeshes()[ind].Transform;

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		};

		// X Axis
		Ref<PhysicsActor> actor;
		if (m_controller.SelectedAxis == MotionCtlAxis_X || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_selectCoord->XAxisEnt->getActor(actor), "onDrawMoveCtl: fail to get X axis's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_selectCoord->XAxisEnt->getComponent<C3dUIComponent>(), pos);
		}

		// Y Axis
		if (m_controller.SelectedAxis == MotionCtlAxis_Y || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_selectCoord->YAxisEnt->getActor(actor), "onDrawMoveCtl: fail to get Y axis's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_selectCoord->YAxisEnt->getComponent<C3dUIComponent>(), pos);
		}	

		// Z Axis
		if (m_controller.SelectedAxis == MotionCtlAxis_Z || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_selectCoord->ZAxisEnt->getActor(actor), "onDrawMoveCtl: fail to get Z axis's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_selectCoord->ZAxisEnt->getComponent<C3dUIComponent>(), pos);
		}
	}

	void MotionControlLayer::onDrawRotationCtl(const glm::vec3 pos)
	{
		CameraComponent &camComp = m_mainCameraEnt->getComponent<CameraComponent>();
		Camera &camera = camComp.Camera;
		const Viewport &vp = camera.getViewport();

		CameraUD &cameraUD = m_motionCamUD;
		Ref<UniformBufferSet> ubSet = m_motionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		cameraUD.ViewMat = camera.getViewMat();
		cameraUD.PrjMat = camera.getPrjMat();
		cameraUD.OrthoMat = camera.getOrthoMat();
		cameraUD.CameraPosition = camera.getCameraPos();
		cameraUD.FrustumFar = camera.far();
		cameraUD.serialize();

		Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
		{
			Renderer::CommitViewport(Viewport{ vp.X, vp.Y, vp.Width, vp.Height });
			cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
		});

		auto drawFunc = [pos, ubSet](C3dUIComponent &meshComp)
		{
			Ref<StaticMesh> axis = AssetsManager::GetRuntimeAsset<StaticMesh>(meshComp.Handle);
			Ref<MeshSource> meshSrc = AssetsManager::GetRuntimeAsset<MeshSource>(meshComp.SourceHandle);
			uint32_t ind = meshComp.SubmeshIndex;
			RenderState state = meshComp.State;

			Ref<MaterialTable> mt = axis->getMaterialTable();
			glm::mat4 transform = glm::translate(glm::mat4(1.f), pos);
			transform = transform * meshSrc->getSubmeshes()[ind].Transform;

			Renderer::SubmitRC([ubSet, axis, ind, transform, mt, state]() mutable
			{
				AssetHandle h = axis->getHandle();
				Renderer::RenderSubmesh(h, ind, mt, Ref<RenderPass>(), ubSet, transform, state);
			});
		};

		Ref<PhysicsActor> actor;
		if (m_controller.SelectedAxis == MotionCtlAxis_X || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_rotateCtl->XTorus->getActor(actor), "onDrawRotationCtl: fail to get X torus's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_rotateCtl->XTorus->getComponent<C3dUIComponent>());
		}

		if (m_controller.SelectedAxis == MotionCtlAxis_Y || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_rotateCtl->YTorus->getActor(actor), "onDrawRotationCtl: fail to get Y torus's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_rotateCtl->YTorus->getComponent<C3dUIComponent>());
		}

		if (m_controller.SelectedAxis == MotionCtlAxis_Z || m_controller.SelectedAxis == MotionCtlAxis_Num)
		{
			PIO_ASSERT_RETURN(m_rotateCtl->ZTorus->getActor(actor), "onDrawRotationCtl: fail to get Z torus's actor");
			actor->setGlobalPose(pos);
			drawFunc(m_rotateCtl->ZTorus->getComponent<C3dUIComponent>());
		}
	}

	void MotionControlLayer::onDrawUIDistantLight(const glm::vec3 &pos)
	{
		Ref<UniformBufferSet> ubSet = m_motionUBSet;
		auto &uiComp = m_uiDistantLight->Mesh->getComponent<C3dUIComponent>();
		auto &transComp = m_uiDistantLight->Mesh->getComponent<TransformComponent>();

		AssetHandle h = uiComp.Handle;
		RenderState &state = uiComp.State;
		glm::mat4 trans = glm::translate(glm::mat4(1.f), pos) * transComp.mat();

		Renderer::SubmitRC([ubSet, h, state, trans]() mutable
		{
			Renderer::RenderLine(h, ubSet, trans, state);
		});
	}

	void MotionControlLayer::onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const WindowLayoutParams &param)
	{
		if (!ctlActor || !selection)
			return;

		Ref<Entity> ctlEnt;
		PIO_ASSERT_RETURN(ctlActor->getEnt(ctlEnt), "onSelectionMoved: entity is invalid");

		glm::ivec2 curPt = UiDef::ScreenToViewport(cursor, param);
		glm::ivec2 lastPt = UiDef::ScreenToViewport(last, param);

		Ray curRay = Ray::BuildFromScreen(curPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
		Ray lastRay = Ray::BuildFromScreen(lastPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);

		glm::vec3 dirDiff = curRay.Dir - lastRay.Dir;

		auto &uiComp = ctlEnt->getComponent<C3dUIComponent>();
		glm::vec3 diff3d(0.f);
		std::string_view ctlName;
		if (std::strcmp(uiComp.Name.data(), UI_AXIS_X) == 0)
		{
			diff3d.x = dirDiff.x;
			ctlName = UI_AXIS_X;
			m_controller.SelectedAxis = MotionCtlAxis_X;
		}
		else if (std::strcmp(uiComp.Name.data(), UI_AXIS_Y) == 0)
		{
			diff3d.y = dirDiff.y;
			ctlName = UI_AXIS_Y;
			m_controller.SelectedAxis = MotionCtlAxis_Y;
		}
		else if (std::strcmp(uiComp.Name.data(), UI_AXIS_Z) == 0)
		{
			diff3d.z = dirDiff.z;
			ctlName = UI_AXIS_Z;
			m_controller.SelectedAxis = MotionCtlAxis_Z;
		}
		else if (std::strcmp(uiComp.Name.data(), UI_TORUS_X) == 0)
		{
			diff3d.x = -dirDiff.y;
			ctlName = UI_TORUS_X;
			m_controller.SelectedAxis = MotionCtlAxis_X;
		}
		else if (std::strcmp(uiComp.Name.data(), UI_TORUS_Y) == 0)
		{
			diff3d.y = dirDiff.x;
			ctlName = UI_TORUS_Y;
			m_controller.SelectedAxis = MotionCtlAxis_Y;
		}
		else if (std::strcmp(uiComp.Name.data(), UI_TORUS_Z) == 0)
		{
			diff3d.z = -dirDiff.x;
			ctlName = UI_TORUS_Z;
			m_controller.SelectedAxis = MotionCtlAxis_Z;
		}
		else
		{
			LOGE("invalid selection");
			return;
		}

		if (m_controller.bMode(MotionCtl_Move))
		{
			onMoveMode(selection, diff3d * CTL_TRANSLATION_RATIO, ctlName);
		}
		else if (m_controller.bMode(MotionCtl_Rotation))
		{			
			onRotateMode(selection, diff3d * CTL_ROTATE_RATIO, ctlName);
		}		
	}

	bool MotionControlLayer::onHandleClick(const glm::vec2 &winCursor)
	{		
		glm::vec2 screenCursor = winCursor - glm::vec2(m_layoutParam.Position.Left, m_layoutParam.Position.Top);
		if (m_viewIconsRect.contain(screenCursor.x, screenCursor.y))
		{
			if (onHandleIconClick(screenCursor)) 
			{ 			
				return true; 
			}
		}

		if (onHandleObject3dClick(winCursor))
		{		
			m_controller.SelectedSprite.reset();
			return true;
		}

		if (onHandleSpriteClick(winCursor))
		{
			m_controller.SelectedObj3D.reset();
			return true;
		}

		return false;
	}

	bool MotionControlLayer::onHandleIconClick(const glm::vec2 &cursor)
	{
		for (uint8_t i = 0; i < MotionCtl_Num; i++)
		{
			if (m_views[i] && m_views[i]->contains(cursor) && m_controller.SelectedView != m_views[i])
			{
				if (m_controller.SelectedView)
				{
					m_controller.SelectedView->setStatus(ViewCtlStatus_Normal);
					m_controller.SelectedView->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_NORMAL[m_controller.Mode]));
				}
				m_views[i]->setStatus(ViewCtlStatus_Selected);
				m_views[i]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_SELECT[i]));

				m_controller.SelectedView = m_views[i];
				m_controller.Mode = MotionCtlMode(i);
				return true;
			}
		}
		return false;
	}

	void MotionControlLayer::onMoveMode(Ref<Entity> &ent, const glm::vec3 &diff, const std::string_view &ctlName)
	{
		if (ent->hasComponent<ProxyComponent>())
		{
			auto &comp = ent->getComponent<ProxyComponent>();
			comp.Proxy->setGlobalPoseDiff(diff);
		}
		else if (ent->hasComponent<DirectionalLightComponent>())
		{
			auto &comp = ent->getComponent<TransformComponent>();
			comp.Transform.Position += diff;
		}
		else
		{
			ent->setGlobalPoseDiff(diff);
		}
	}

	void MotionControlLayer::onRotateMode(Ref<Entity> &ent, const glm::vec3 &eulerDiff, const std::string_view &ctlName)
	{
		if (ent->hasComponent<ProxyComponent>())
		{
			auto &comp = ent->getComponent<ProxyComponent>();
			comp.Proxy->setGlobalPoseDiff(glm::vec3(0.f), eulerDiff);
		}
		else if (ent->hasComponent<DirectionalLightComponent>())
		{
			auto &comp = ent->getComponent<TransformComponent>();
			comp.Transform.Euler += eulerDiff;
		}
		else
		{
			ent->setGlobalPoseDiff(glm::vec3(0.f), eulerDiff);
		}
	}

	bool MotionControlLayer::onHandleObject3dClick(const glm::vec2 &winCursor)
	{
		glm::ivec2 viewportPt = UiDef::ScreenToViewport(winCursor, m_layoutParam);
		Ray r = Ray::BuildFromScreen(viewportPt, m_mainCameraEnt->getComponent<CameraComponent>().Camera);
		auto &sceneComp = m_sceneEnt->getComponent<SceneComponent>();
		HitResult result = AssetsManager::GetRuntimeAsset<PhysicsScene>(sceneComp.PhycisScene)->intersect(r);
		if (m_controller.SelectedObj3D) { m_controller.SelectedObj3D->setSelection(false); }
		bool consume = result.Hit && result.Actor->getEnt(m_controller.SelectedObj3D);
		if (consume)
		{
			LOGD("hit entity in scene");
			m_controller.SelectedObj3D->setSelection(true);
		}
		else
		{
			m_controller.SelectedObj3D.reset();
		}
		return consume;
	}

	bool MotionControlLayer::onHandleSpriteClick(const glm::vec2 &winCursor)
	{
		glm::vec2 vpCursor = winCursor - glm::vec2(m_layoutParam.Position.Left, m_layoutParam.Position.Top);
		EntityView view = s_registry->view<SpriteComponent>();
		auto it = view.begin();
		while (it != view.end())
		{
			Ref<Entity> ent = it->second;
			SpriteComponent &spriteComp = ent->getComponent<SpriteComponent>();
			bool consume = Math::Contains(vpCursor, spriteComp.Rect);
			if (consume)
			{
				m_controller.SelectedSprite = ent;
				return consume;
			}
			it++;
		}
		m_controller.SelectedSprite.reset();
		return false;
	}
}