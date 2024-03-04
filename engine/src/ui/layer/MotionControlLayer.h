#ifndef __PIONNER_UI_LAYER_C3D_CONTROL_H__
#define __PIONNER_UI_LAYER_C3D_CONTROL_H__

#include "Layer.h"

#include "gfx/struct/Camera.h"
#include "physics/PhysicsDef.h"

namespace pio
{
	class MeshSource;
	class UniformBufferSet;
	class UiCoordinate3D;
	class UiRotationCtl;
	class UiDistantLight;
	class PhysicsScene;
	class View;

	enum MotionCtlMode : uint8_t
	{
		MotionCtl_Move = 0, MotionCtl_Rotation, MotionCtl_Scale, MotionCtl_Num
	};

	enum MotionTarget : uint8_t
	{
		MotionTarget_None = 0, MotionTarget_Icon, MotionTarget_Vision, MotionTarget_Object3D, MotionTarget_Sprite
	};

	enum MotionCtlAxis : uint8_t
	{
		MotionCtlAxis_X, MotionCtlAxis_Y, MotionCtlAxis_Z, MotionCtlAxis_Num
	};

	class MotionController
	{
	public:
		MotionController() {}
		~MotionController() = default;

	private:
		bool bTarget(MotionTarget t) { return Target == t; }
		void setTarget(MotionTarget t) { Target = t; }

		bool bMode(MotionCtlMode m) { return Mode == m; }

	private:
		static bool IsClick(uint64_t now, uint64_t pre) { return (now - pre) <= CLICK_INTERVAL; }

	private:
		Ref<View> SelectedView;
		Ref<Entity> SelectedObj3D;
		Ref<Entity> SelectedSprite;
		MotionCtlAxis SelectedAxis{ MotionCtlAxis_Num };
		MotionCtlMode Mode{ MotionCtl_Move };
		MotionTarget Target{ MotionTarget_None };		
		PhysicsActor *CtlActor{ nullptr };
		bool CtlActorPressed{ false };

		uint64_t DownTime{ 0 };// ms
		glm::vec2 LastWinCursor{ -1.f };

	private:
		friend class MotionControlLayer;
	};

	class MotionControlLayer : public Layer
	{
	public:
		MotionControlLayer(const WindowLayoutParams &param);
		virtual ~MotionControlLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual bool onEvent(Event &event) override;
		virtual void onUpdate(const Timestep &ts) override;
		virtual void onWindowSizeChange(uint32_t width, uint32_t height) override;

	private:
		bool onMouseButtonPressed(Event &event);
		bool onMouseMoved(Event &event);
		bool onMouseButtonReleased(Event &event);
		bool onMouseScrolled(Event &event);

		void onDrawVisionCtl(const Timestep &ts);
		void onDrawMotionCtl(const Timestep &ts);
		void onDrawMotionView(const Timestep &ts);

		void onDrawMoveMode(const Timestep &ts);
		void onDrawRotationMode(const Timestep &ts);

		void onDrawMoveCtl(const glm::vec3 pos);
		void onDrawRotationCtl(const glm::vec3 pos);
		void onDrawUIDistantLight(const glm::vec3 &pos);

		void onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const WindowLayoutParams &param);
		void onMoveMode(Ref<Entity> &ent, const glm::vec3 &diff, const std::string_view &ctlName);
		void onRotateMode(Ref<Entity> &ent, const glm::vec3 &eulerDiff, const std::string_view &ctlName);

		bool onHandleClick(const glm::vec2 &winCursor);
		bool onHandleIconClick(const glm::vec2 &cursor);
		bool onHandleObject3dClick(const glm::vec2 &winCursor);
		bool onHandleSpriteClick(const glm::vec2 &winCursor);

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
		Ref<UiRotationCtl> m_rotateCtl;
		Ref<UiDistantLight> m_uiDistantLight;

		Ref<PhysicsScene> m_motionCtlPhysx[MotionCtl_Num];

		MotionController m_controller;
		Ref<View> m_views[MotionCtl_Num];
		LayoutRect m_viewIconsRect;
	};
}

#endif