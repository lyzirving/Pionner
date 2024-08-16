#include "MotionControlLayer.h"

#include "Application.h"

#include "gfx/rhi/UniformBufferSet.h"
#include "gfx/rhi/Texture.h"
#include "gfx/renderer/Renderer.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/Geometry2D.h"
#include "gfx/debug/GDebugger.h"

#include "physics/PhysicsScene.h"
#include "physics/PhysicsActor.h"

#include "scene/Scene.h"
#include "scene/Registry.h"
#include "scene/Components.h"

#include "ui/Ui3D.h"
#include "ui/View.h"
#include "ui/editor/GizmoTransform.h"
#include "ui/editor/GizmoRotator.h"

#include "asset/AssetsManager.h"
#include "event/MouseEvent.h"

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

	MotionControlLayer::MotionControlLayer(const LayoutParams &param)
		: Layer(param, "MotionControlLayer")
	{
	}

	void MotionControlLayer::onAttach()
	{
		m_circleLayoutParam.Ratio = LayoutRatio(m_layoutParam.Ratio.Right - 0.1f,
												m_layoutParam.Ratio.Top + 0.015f,
												m_layoutParam.Ratio.Right,
												m_layoutParam.Ratio.Top + 0.125f);

		m_views[MotionCtl_Move] = CreateRef<View>("Move Ctl View");
		m_views[MotionCtl_Move]->setStatus(ViewCtlStatus_Selected);
		m_views[MotionCtl_Move]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_SELECT[MotionCtl_Move]));

		m_views[MotionCtl_Rotation] = CreateRef<View>("Rotate Ctl View");
		m_views[MotionCtl_Rotation]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_NORMAL[MotionCtl_Rotation]));

		MotionController::SelectView(m_views[MotionCtl_Move]);
		MotionController::Mode(MotionCtl_Move);

		m_visionCamUD.obtainBlock();
		m_visionUBSet = UniformBufferSet::Create();
		m_visionUBSet->create(m_visionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);
		m_visionCam = Camera::Main->clone();

		m_motionCamUD.obtainBlock();
		m_motionUBSet = UniformBufferSet::Create();
		m_motionUBSet->create(m_motionCamUD.Block.getByteUsed(), (uint32_t)UBBindings::Camera);

		UiCoordinate3DBuilder visionBuilder;
		visionBuilder.Shape = CoordinateShape::Cylinder;
		m_visionCoord = CreateRef<UiCoordinate3D>(visionBuilder);

		m_uiDistantLight = CreateRef<UiDistantLight>(0.5f, 2.f, glm::vec4(0.964f, 0.953f, 0.051f, 1.f));
		m_uiPointLight = CreateRef<UiPointLight>(0.5f, glm::vec4(0.964f, 0.953f, 0.051f, 1.f));

		m_gizmoTransform = CreateRef<GizmoTransform>();
		m_gizmoRotator = CreateRef<GizmoRotator>();

		onWindowSizeChange(Application::MainWindow()->width(),
						   Application::MainWindow()->height());
	}

	void MotionControlLayer::onDetach()
	{
		m_visionUBSet.reset();
		m_motionUBSet.reset();
		m_visionCoord.reset();
		m_uiDistantLight.reset();
		m_uiPointLight.reset();
	}

	bool MotionControlLayer::onEvent(Event &event)
	{
	#if 0
		EventDispatcher dispatcher(event);

		dispatcher.dispatch<MouseButtonPressedEvent>(PIO_BIND_FN_SELF(MotionControlLayer::onMouseButtonPressed, std::placeholders::_1));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseMovedEvent>(PIO_BIND_FN_SELF(MotionControlLayer::onMouseMoved, std::placeholders::_1));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseButtonReleasedEvent>(PIO_BIND_FN_SELF(MotionControlLayer::onMouseButtonReleased, std::placeholders::_1));
		if (event.Handled) return true;

		dispatcher.dispatch<MouseScrolledEvent>(PIO_BIND_FN_SELF(MotionControlLayer::onMouseScrolled, std::placeholders::_1));
		if (event.Handled) return true;
	#endif // 0

		return false;
	}

	void MotionControlLayer::onUpdate(const Timestep &ts)
	{
		onDrawVisionCtl(ts);
		onDrawMotionCtl(ts);
		onDrawMotionView(ts);
	}

	void MotionControlLayer::onUpdateUI(const Timestep &ts)
	{
		// Circle background
		if (m_drawCircle)
		{
			Rect2d &c = m_circleLayoutParam.Position;
			Renderer::SubmitRC([c]() mutable
			{
				ImDrawList *drawList = ImGui::GetForegroundDrawList();
				drawList->AddCircleFilled(ImVec2(c.Left + c.width() * 0.5f, c.Top + c.height() * 0.5f), c.height() * 0.5f, ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.3f)));
			});
		}
	}

	void MotionControlLayer::onWindowSizeChange(uint32_t width, uint32_t height)
	{
		m_layoutParam.calculate(width, height);
		m_circleLayoutParam.calculate(width, height);

		Rect2d lp = m_circleLayoutParam.Position;
		float radius = (lp.Bottom - lp.Top) * 0.5f;
		glm::vec2 center(lp.Right - radius - 3, (lp.Top + lp.Bottom) * 0.5f);

		m_circleLayoutParam.Position.Left = center.x - radius;
		m_circleLayoutParam.Position.Right = center.x + radius;
		m_circleLayoutParam.Position.Top = center.y - radius;
		m_circleLayoutParam.Position.Bottom = center.y + radius;

		m_circleLayoutParam.Viewport.setW(m_circleLayoutParam.Position.Right - m_circleLayoutParam.Position.Left);
		m_circleLayoutParam.Viewport.setH(m_circleLayoutParam.Position.Bottom - m_circleLayoutParam.Position.Top);
		m_circleLayoutParam.Viewport.setX(m_circleLayoutParam.Position.Left);
		m_circleLayoutParam.Viewport.setY(height - m_circleLayoutParam.Position.Top - m_circleLayoutParam.Viewport.h());

		m_gizmoTransform->setLayoutParam(m_layoutParam);
		m_gizmoRotator->setLayoutParam(m_layoutParam);

		uint32_t l{ 10 }, t{ 5 }, viewWid{ 30 }, viewHeight{ 30 };
		m_views[MotionCtl_Move]->setPosition(l, t, viewWid, viewHeight);
		m_views[MotionCtl_Rotation]->setPosition(l + viewWid, t, viewWid, viewHeight);

		m_views[MotionCtl_Move]->setViewport(0, 0, m_layoutParam.Viewport.w(), m_layoutParam.Viewport.h());
		m_views[MotionCtl_Rotation]->setViewport(0, 0, m_layoutParam.Viewport.w(), m_layoutParam.Viewport.h());

		m_viewIconsRect = m_views[MotionCtl_Move]->getRect();
		for (uint8_t i = MotionCtl_Rotation; i < MotionCtl_Num; i++)
		{
			if (m_views[i])
				m_viewIconsRect.doUnion(m_views[i]->getRect());
		}
	}

	bool MotionControlLayer::onMouseButtonPressed(Event &event)
	{
		auto *e = event.as<MouseButtonPressedEvent>();
		m_winCursor.x = e->getCursorX(); m_winCursor.y = e->getCursorY();
		m_downTime = TimeUtil::CurrentTimeMs();

		if (onMouseBtnPressVisionControl(event))
			return true;

		if (onMouseBtnPressSceneItem(event))
			return true;

		return false;
	}

	bool MotionControlLayer::onMouseButtonReleased(Event &event)
	{
		bool bUsing = !MotionController::bTarget(MotionTarget_None);
		if (!bUsing && MotionController::bClick(TimeUtil::CurrentTimeMs(), m_downTime))
		{
			bUsing = onHandleClick(Application::MainWindow()->cursor());
		}

		if (MotionController::bTarget(MotionTarget_Vision)) { Application::MainWindow()->setCursorMode(CursorMode::Normal); }
		
		MotionController::SelectAxis(MotionCtlAxis_Num);
		MotionController::Target(MotionTarget_None);

		m_gizmoTransform->onMouseButtonReleased(event);
		m_gizmoRotator->onMouseButtonReleased(event);
		m_winCursor.x = m_winCursor.y = -1.f;
		m_downTime = 0;

		return bUsing;
	}

	bool MotionControlLayer::onMouseMoved(Event &event)
	{
		if (onMouseMoveVisionControl(event))
			return true;

		if (onMouseMoveSceneItem(event))
			return true;

		return false;
	}

	bool MotionControlLayer::onMouseScrolled(Event &event)
	{
		glm::vec2 cursor = Application::MainWindow()->cursor();
		bool inside = m_circleLayoutParam.Position.contain((uint32_t)cursor.x, (uint32_t)cursor.y);
		if (inside)
		{
			auto *p = event.as<MouseScrolledEvent>();						
			Camera::Main->setPositionDelta(SphereCoord(0.f, 0.f, -p->getYOffset() * 0.5f/*scale factor*/));
		}
		return inside;
	}

	void MotionControlLayer::onDrawVisionCtl(const Timestep &ts)
	{				
		const Viewport &vp = m_circleLayoutParam.Viewport;

		CameraUD &cameraUD = m_visionCamUD;
		Ref<UniformBufferSet> ubSet = m_visionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		SphereCoord camPos = Camera::Main->position();
		// Make ui camera's radius remain unchange
		camPos.setRadius(CTL_CAM_RADIUS);
		m_visionCam.setPosition(camPos);
		m_visionCam.flush();

		cameraUD.ViewMat = m_visionCam.viewMat();
		cameraUD.PrjMat = m_visionCam.prjMat();
		cameraUD.OrthoMat = m_visionCam.orthoMat();
		cameraUD.CameraPosition = m_visionCam.position();
		cameraUD.FrustumFar = m_visionCam.frustFar();
		cameraUD.serialize();

		Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
		{
			Renderer::CommitViewport(vp);
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
		const Viewport &vp = Camera::Main->viewport();

		CameraUD &cameraUD = m_motionCamUD;
		Ref<UniformBufferSet> ubSet = m_motionUBSet;
		Ref<UniformBuffer> cameraUB = ubSet->get((uint32_t)UBBindings::Camera);

		cameraUD.ViewMat = Camera::Main->viewMat();
		cameraUD.PrjMat = Camera::Main->prjMat();
		cameraUD.OrthoMat = Camera::Main->orthoMat();
		cameraUD.CameraPosition = Camera::Main->position();
		cameraUD.FrustumFar = Camera::Main->frustFar();
		cameraUD.serialize();

		showGizmo(MotionController::bSpriteSelectd() || MotionController::bObj3dSelectd());

		Renderer::SubmitRC([vp, cameraUB, cameraUD]() mutable
		{
			Renderer::CommitViewport(vp);
			cameraUB->setData(cameraUD.Block.getBuffer()->as<void *>(), cameraUD.Block.getByteUsed());
		});

		switch (MotionController::GetMode())
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

		GDebugger::Get()->drawLine(ubSet, RenderState(Blend::Disable(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable()));
	}

	void MotionControlLayer::onDrawMotionView(const Timestep &ts)
	{		
		const Viewport &vp = Camera::Main->viewport();

		Renderer::SubmitRC([vp]() mutable
		{
			Renderer::CommitViewport(vp);
		});
		
		m_viewDrawCmds.clear();
		m_viewDrawCmds.reserve(MotionCtl_Num);
		RenderState s{ Blend::Disable(), DepthTest::Always(), CullFace::Common(), StencilTest::Disable() };
		for (uint8_t i = 0; i < MotionCtl_Num; i++)
		{
			if (!m_views[i]) continue;

			m_views[i]->upload();
			m_viewDrawCmds.emplace_back(m_views[i]->getMesh(), m_views[i]->getTexture(), s, false);
		}

		std::vector<TextureCmd> &cmds = m_viewDrawCmds;
		Renderer::SubmitRC([cmds]() mutable
		{
			Renderer::RenderSprites(cmds);
		});
	}

	void MotionControlLayer::onDrawMoveMode(const Timestep &ts)
	{
		if (MotionController::bSpriteSelectd())
		{
			if (MotionController::GetSprite()->hasComponent<DirectionalLightComponent>())
			{
				auto &lightComp = MotionController::GetSprite()->getComponent<DirectionalLightComponent>();
				auto &transComp = MotionController::GetSprite()->getComponent<TransformComponent>();
				m_gizmoTransform->setTranslation(transComp.Transform.Position);
				m_gizmoTransform->onDraw(DrawParam{ ts, m_motionUBSet });
				onDrawUIDistantLight(lightComp, transComp);				
			}
			else if (MotionController::GetSprite()->hasComponent<PointLightComponent>())
			{
				auto &lightComp = MotionController::GetSprite()->getComponent<PointLightComponent>();
				auto &transComp = MotionController::GetSprite()->getComponent<TransformComponent>();
				m_gizmoTransform->setTranslation(transComp.Transform.Position);
				m_gizmoTransform->onDraw(DrawParam{ ts, m_motionUBSet });
				onDrawUIPointLight(lightComp, transComp);				
			}
		}
		else if (MotionController::bObj3dSelectd())
		{
			glm::vec3 ctlPos(0.f);
			MotionController::GetObj3D()->getGlobalPose(ctlPos);
			m_gizmoTransform->setTranslation(ctlPos);
			m_gizmoTransform->onDraw(DrawParam{ ts, m_motionUBSet });
		}
	}

	void MotionControlLayer::onDrawRotationMode(const Timestep &ts)
	{
		if (MotionController::bSpriteSelectd())
		{
			if (MotionController::GetSprite()->hasComponent<DirectionalLightComponent>())
			{
				auto &lightComp = MotionController::GetSprite()->getComponent<DirectionalLightComponent>();
				auto &transComp = MotionController::GetSprite()->getComponent<TransformComponent>();				
				m_gizmoRotator->setTranslation(transComp.Transform.Position);
				m_gizmoRotator->onDraw(DrawParam{ ts, m_motionUBSet });
				onDrawUIDistantLight(lightComp, transComp);				
			}
			else if (MotionController::GetSprite()->hasComponent<PointLightComponent>())
			{
				auto &lightComp = MotionController::GetSprite()->getComponent<PointLightComponent>();
				auto &transComp = MotionController::GetSprite()->getComponent<TransformComponent>();
				m_gizmoRotator->setTranslation(transComp.Transform.Position);
				m_gizmoRotator->onDraw(DrawParam{ ts, m_motionUBSet });
				onDrawUIPointLight(lightComp, transComp);				
			}
		}
		else if (MotionController::bObj3dSelectd())
		{
			glm::vec3 ctlPos(0.f);
			MotionController::GetObj3D()->getGlobalPose(ctlPos);
			m_gizmoRotator->setTranslation(ctlPos);
			m_gizmoRotator->onDraw(DrawParam{ ts, m_motionUBSet });
		}
	}

	void MotionControlLayer::onDrawUIDistantLight(DirectionalLightComponent &lightComp, TransformComponent &transComp)
	{
		const glm::vec3 &pos = transComp.Transform.Position;
		Ref<UniformBufferSet> ubs = m_motionUBSet;

		auto &uiComp = m_uiDistantLight->Mesh->getComponent<C3dUIComponent>();

		AssetHandle h = uiComp.Handle;
		glm::vec4 &c = m_uiDistantLight->Color;
		RenderState &state = uiComp.State;
		glm::vec3 dest = glm::normalize(transComp.Transform.Euler.mat() * glm::vec4(lightComp.Direction, 0.f));
		glm::quat rot = quaternion::RotationToQuat(AXIS_Z, dest);
		glm::mat4 trans = glm::translate(glm::mat4(1.f), pos) * glm::toMat4(rot);

		Renderer::SubmitRC([h, c, ubs, state, trans]() mutable
		{			
			Renderer::RenderLineSegment(h, c, ubs, trans, state);
		});
	}

	void MotionControlLayer::onDrawUIPointLight(PointLightComponent &lightComp, TransformComponent &transComp)
	{
		const glm::vec3 &pos = transComp.Transform.Position;
		Ref<UiPointLight> ui = m_uiPointLight;
		glm::vec4 &c = ui->Color;
		Ref<UniformBufferSet> ubs = m_motionUBSet;

		auto &uiComp = ui->Mesh->getComponent<C3dUIComponent>();
		AssetHandle h = uiComp.Handle;
		RenderState &state = uiComp.State;		

		if (ui->setRadius(lightComp.Radius))
		{
			Renderer::SubmitTask([ui]() { ui->upload(); });
		}

		Renderer::SubmitRC([h, c, ubs, state, pos]() mutable
		{			
			Renderer::RenderLineSegment(h, c, ubs, glm::translate(glm::mat4(1.f), pos), state);
		});
	}

	bool MotionControlLayer::onHandleClick(const glm::vec2 &winCursor)
	{
		if (onHandleIconClick(winCursor))
			return true;

		if (onHandleSpriteClick(winCursor))
		{
			if (MotionController::bObj3dSelectd()) { MotionController::GetObj3D()->setSelection(false); }
			MotionController::SelectObj3D(nullptr);
			return true;
		}

		if (onHandleObject3dClick(winCursor))
		{
			MotionController::SelectSprite(nullptr);
			return true;
		}
		return false;
	}

	bool MotionControlLayer::onHandleIconClick(const glm::vec2 &winCursor)
	{
		glm::vec2 screenPt = winCursor - glm::vec2(m_layoutParam.Position.Left, m_layoutParam.Position.Top);
		if (!m_viewIconsRect.contain(screenPt.x, screenPt.y))
			return false;

		for (uint8_t i = 0; i < MotionCtl_Num; i++)
		{
			if (m_views[i] && m_views[i]->contains(screenPt) && !MotionController::bSelectedView(m_views[i]))
			{
				if (MotionController::bSelectedView())
				{
					MotionController::GetView()->setStatus(ViewCtlStatus_Normal);
					MotionController::GetView()->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_NORMAL[MotionController::GetMode()]));
				}
				m_views[i]->setStatus(ViewCtlStatus_Selected);
				m_views[i]->setTexture(AssetsManager::GetPackedAsset<Texture2D>(ICON_ID_SELECT[i]));

				MotionController::SelectView(m_views[i]);
				MotionController::Mode(MotionCtlMode(i));
				return true;
			}
		}
		return false;
	}

	bool MotionControlLayer::onHandleSpriteClick(const glm::vec2 &winCursor)
	{
		glm::vec2 screenPt = winCursor - glm::vec2(m_layoutParam.Position.Left, m_layoutParam.Position.Top);

		EntityView view = s_registry->view<SpriteComponent>();
		auto it = view.begin();
		while (it != view.end())
		{
			Ref<Entity> ent = it->second;
			SpriteComponent &spriteComp = ent->getComponent<SpriteComponent>();			
			bool consume = spriteComp.Rect.contain(screenPt.x, screenPt.y);
			if (consume)
			{
				MotionController::SelectSprite(ent);
				return consume;
			}
			it++;
		}
		MotionController::SelectSprite(nullptr);
		return false;
	}

	bool MotionControlLayer::onHandleObject3dClick(const glm::vec2 &winCursor)
	{
		Ray ray = Camera::Main->screenPointToRay(winCursor, m_layoutParam, true);

		HitResult result = Scene::Main->physics()->intersect(ray);
		if (MotionController::bObj3dSelectd()) { MotionController::GetObj3D()->setSelection(false); }
		Ref<Entity> select3d;
		bool consume = result.Hit && result.Actor->getEnt(select3d);
		if (consume && select3d)
		{
			LOGD("hit entity in scene");
			MotionController::SelectObj3D(select3d);
			MotionController::GetObj3D()->setSelection(true);
		}
		else
		{
			MotionController::SelectObj3D(nullptr);
		}
		return consume;
	}

	bool MotionControlLayer::onMouseBtnPressVisionControl(Event &event)
	{
		auto *e = event.as<MouseButtonPressedEvent>();
		glm::vec2 winCursor{ e->getCursorX(), e->getCursorY() };
		if (m_circleLayoutParam.Position.contain(winCursor.x, winCursor.y))
		{
			MotionController::Target(MotionTarget_Vision);
			Application::MainWindow()->setCursorMode(CursorMode::Disabled);
			return true;
		}
		return false;
	}

	bool MotionControlLayer::onMouseBtnPressSceneItem(Event &event)
	{
		if (MotionController::bObj3dSelectd() || MotionController::bSpriteSelectd())
		{
			MotionCtlMode mode = MotionController::GetMode();
			bool consume{ false };
			switch (mode)
			{
				case MotionCtlMode::MotionCtl_Move:
				{
					consume = m_gizmoTransform->onMouseButtonPressed(event);
					break;
				}
				case MotionCtlMode::MotionCtl_Rotation:
				{
					consume = m_gizmoRotator->onMouseButtonPressed(event);
					break;
				}
				default:
					LOGE("invalid MotionCtlMode[%u]", mode);
					break;
			}
			if (consume) 
			{ 
				if(MotionController::bObj3dSelectd())
					MotionController::Target(MotionTarget_Object3D);
				else if (MotionController::bSpriteSelectd())
					MotionController::Target(MotionTarget_Sprite);
			}
			return consume;
		}
		return false;
	}

	bool MotionControlLayer::onMouseMoveVisionControl(Event &event)
	{
		auto *p = event.as<MouseMovedEvent>();
		if (MotionController::bTarget(MotionTarget_Vision))
		{
			glm::vec2 winCursor(p->getCursorX(), p->getCursorY());
			m_drawCircle = true;
			glm::vec2 delta = winCursor - m_winCursor;
			delta.x = Math::Clamp(delta.x, -20.f, 20.f);
			delta.y = Math::Clamp(delta.y, -20.f, 20.f);
			Camera::Main->setPositionDelta(SphereCoord(-delta.y, -delta.x, 0.f) * 0.75f);			
			m_winCursor = winCursor;
			return true;
		}
		m_drawCircle = m_circleLayoutParam.Position.contain(p->getCursorX(), p->getCursorY());
		return false;
	}

	bool MotionControlLayer::onMouseMoveSceneItem(Event &event)
	{
		MotionCtlMode mode = MotionController::GetMode();
		if (MotionController::bTarget(MotionTarget_Object3D) || MotionController::bTarget(MotionTarget_Sprite))
		{
			switch (mode)
			{
				case MotionCtlMode::MotionCtl_Move:
				{
					if (m_gizmoTransform->onMouseMoved(event))
					{
						if(MotionController::bTarget(MotionTarget_Object3D))
							onSelectionMove(MotionController::GetObj3D(), m_gizmoTransform->getTransferDiff());
						else if(MotionController::bTarget(MotionTarget_Sprite))
							onSelectionMove(MotionController::GetSprite(), m_gizmoTransform->getTransferDiff());
					}
					break;
				}
				case MotionCtlMode::MotionCtl_Rotation:
				{
					if (m_gizmoRotator->onMouseMoved(event))
					{
						if (MotionController::bTarget(MotionTarget_Object3D))
							onSelectionRotate(MotionController::GetObj3D(), m_gizmoRotator->getEulerDiff());
						else if (MotionController::bTarget(MotionTarget_Sprite))
							onSelectionRotate(MotionController::GetSprite(), m_gizmoRotator->getEulerDiff());
					}
					break;
				}
				default:
					LOGE("invalid MotionCtlMode[%u]", mode);
					break;
			}
			auto *p = event.as<MouseMovedEvent>();
			m_winCursor.x = p->getCursorX(); m_winCursor.y = p->getCursorY();
			return true;
		}
		else
		{
			switch (mode)
			{
				case MotionCtlMode::MotionCtl_Move:
				{
					m_gizmoTransform->onMouseMoveHovering(event);
					break;
				}
				case MotionCtlMode::MotionCtl_Rotation:
				{
					m_gizmoRotator->onMouseMoveHovering(event);
					break;
				}
				default:
					break;
			}
		}
		return false;
	}

	void MotionControlLayer::onSelectionMove(Ref<Entity> &ent, const glm::vec3 &diff)
	{
		if (ent->hasComponent<DirectionalLightComponent>())
		{
			auto &comp = ent->getComponent<TransformComponent>();
			comp.Transform.Position += diff;
		}
		else
		{
			ent->setGlobalPoseDiff(diff);
		}
	}

	void MotionControlLayer::onSelectionRotate(Ref<Entity> &ent, const glm::vec3 &eulerDiff)
	{
		if (ent->hasComponent<DirectionalLightComponent>())
		{
			auto &comp = ent->getComponent<TransformComponent>();
			comp.Transform.Euler += eulerDiff;
		}
		else
		{
			ent->setGlobalPoseDiff(glm::vec3(0.f), eulerDiff);
		}
	}

	void MotionControlLayer::showGizmo(bool show)
	{
		m_gizmoTransform->setVisible(false);
		m_gizmoRotator->setVisible(false);

		if (show)
		{
			MotionCtlMode mode = MotionController::GetMode();
			switch (mode)
			{
				case MotionCtl_Move:
					m_gizmoTransform->setVisible(true);
					break;
				case MotionCtl_Rotation:
					m_gizmoRotator->setVisible(true);
					break;
				default:
					break;
			}
		}
	}
}