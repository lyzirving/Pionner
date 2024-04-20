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
	class UiRotationCtl;
	class UiDistantLight;
	class UiPointLight;
	class PhysicsScene;
	class GizmoTransform;
	class Ray;
	struct DirectionalLightComponent;
	struct PointLightComponent;
	struct TransformComponent;

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

		void onDrawMoveCtl(const glm::vec3 &ctlPos);
		void onDrawRotationCtl(const glm::vec3 &ctlPos);
		void onDrawUIDistantLight(DirectionalLightComponent &lightComp, TransformComponent &transComp);
		void onDrawUIPointLight(PointLightComponent &lightComp);

		void onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const WindowLayoutParams &param);
		void onMoveMode(Ref<Entity> &ent, const glm::vec3 &diff, const std::string_view &ctlName);
		void onRotateMode(Ref<Entity> &ent, const glm::vec3 &eulerDiff, const std::string_view &ctlName);

		bool onHandleClick(const glm::vec2 &winCursor);
		bool onHandleIconClick(const glm::vec2 &screenPt);
		bool onHandleSpriteClick(const glm::vec2 &screenPt);
		bool onHandleObject3dClick(const Ray &ray);

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
		Ref<UiCoordinate3D> m_selectCoord;
		Ref<UiRotationCtl>  m_rotateCtl;
		Ref<UiDistantLight> m_uiDistantLight;
		Ref<UiPointLight>   m_uiPointLight;

		Ref<PhysicsScene> m_motionCtlPhysx[MotionCtl_Num];

		Ref<GizmoTransform> m_gizmoTransform;

		Ref<View> m_views[MotionCtl_Num];
		LayoutRect m_viewIconsRect;
	};
}

#endif