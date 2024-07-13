#ifndef __PIONNER_UI_LAYER_C3D_CONTROL_H__
#define __PIONNER_UI_LAYER_C3D_CONTROL_H__

#include "Layer.h"

#include "ui/MotionController.h"
#include "gfx/struct/Camera.h"
#include "physics/PhysicsDef.h"
#include "window/event/EventHandler.h"

namespace pio
{
	class View;
	class MeshSource;
	class UniformBufferSet;
	class UiCoordinate3D;
	class UiDistantLight;
	class UiPointLight;
	class GizmoTransform;
	class GizmoRotator;
	struct DirectionalLightComponent;
	struct PointLightComponent;
	struct TransformComponent;
	struct TextureCmd;

	class MotionControlLayer : public Layer, public EventHandler
	{
	public:
		MotionControlLayer(const WindowLayoutParams &param);
		virtual ~MotionControlLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual bool onEvent(Event &event) override;
		virtual void onUpdate(const Timestep &ts) override;
		virtual void onUpdateUI(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

		virtual bool onMouseButtonPressed(Event &event) override;
		virtual bool onMouseButtonReleased(Event &event) override;
		virtual bool onMouseMoved(Event &event) override;
		virtual bool onMouseScrolled(Event &event) override;

	private:
		void onDrawVisionCtl(const Timestep &ts);
		void onDrawMotionCtl(const Timestep &ts);
		void onDrawMotionView(const Timestep &ts);

		void onDrawMoveMode(const Timestep &ts);
		void onDrawRotationMode(const Timestep &ts);

		void onDrawUIDistantLight(DirectionalLightComponent &lightComp, TransformComponent &transComp);
		void onDrawUIPointLight(PointLightComponent &lightComp);	

		bool onHandleClick(const glm::vec2 &winCursor);
		bool onHandleIconClick(const glm::vec2 &winCursor);
		bool onHandleSpriteClick(const glm::vec2 &winCursor);
		bool onHandleObject3dClick(const glm::vec2 &winCursor);

		bool onMouseBtnPressVisionControl(Event &event);
		bool onMouseBtnPressSceneItem(Event &event);
		bool onMouseMoveVisionControl(Event &event);
		bool onMouseMoveSceneItem(Event &event);

		void onSelectionMove(Ref<Entity> &ent, const glm::vec3 &diff);
		void onSelectionRotate(Ref<Entity> &ent, const glm::vec3 &eulerDiff);

		void showGizmo(bool show);

	private:
		static const std::string ICON_ID_NORMAL[MotionCtl_Num];
		static const std::string ICON_ID_SELECT[MotionCtl_Num];

	private:
		bool m_drawCircle{ false };
		WindowLayoutParams m_circleLayoutParam;

		Ref<Entity> m_mainCameraEnt, m_sceneEnt;

		Ref<UniformBufferSet> m_visionUBSet;
		CameraUD m_visionCamUD;

		Ref<UniformBufferSet> m_motionUBSet;
		CameraUD m_motionCamUD;

		Ref<UiCoordinate3D> m_visionCoord;
		Ref<UiDistantLight> m_uiDistantLight;
		Ref<UiPointLight>   m_uiPointLight;

		Ref<GizmoTransform> m_gizmoTransform;
		Ref<GizmoRotator>   m_gizmoRotator;

		Ref<View> m_views[MotionCtl_Num];
		LayoutRect m_viewIconsRect;
		std::vector<TextureCmd> m_viewDrawCmds;

		uint64_t  m_downTime{ 0 };//ms
		glm::vec2 m_winCursor{ -1.f };
	};
}

#endif