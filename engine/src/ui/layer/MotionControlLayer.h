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
	class PhysicsScene;
	class View;

	enum MotionCtlMode : uint8_t
	{
		MotionCtl_Move = 0, MotionCtl_Rotation, MotionCtl_Scale, MotionCtl_Num
	};

	class MotionController
	{
	public:
		MotionController() {}
		~MotionController() = default;

	private:
		static bool IsClick(uint64_t now, uint64_t pre) { return (now - pre) <= CLICK_INTERVAL; }

	private:
		MotionCtlMode Mode{ MotionCtl_Move };

		Ref<View> SelectedView;
		MotionCtlMode SelectedViewMode{ MotionCtl_Move };

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

		void onDrawMoveCtl(const glm::vec3 pos);
		void onDrawRotationCtl(const glm::vec3 pos);

		void onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const WindowLayoutParams &param);

		bool onHandleClick(const glm::vec2 &cursor);
		void onHandleIconClick(const glm::vec2 &cursor);

	private:
		struct UIEventTracker
		{
			bool Pressed{ false };
			uint64_t PressedTime{ 0 };// ms
			glm::vec2 LastCursor{ -1.f };

			static bool IsClick(uint64_t now, uint64_t pre) { return (now - pre) <= CLICK_INTERVAL; }
		};

		struct SpriteController
		{			
			bool Pressed{ false };
			glm::vec2 LastCursor{ -1.f };
			Ref<Entity> Ent;

			bool bSelected() { return Ent.use_count() != 0; }
			void release() { Ent.reset(); LastCursor.x = LastCursor.y = -1.f; Pressed = false; }
		};

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

		UIEventTracker m_eventCtlState{};
		UIEventTracker m_visionCtlState{};
		UIEventTracker m_objCtlState{};
		SpriteController m_spriteCtl;

		// Physics world that store selector controller's axis
		Ref<PhysicsScene> m_world;
		PhysicsActor *m_hitCtlActor{ nullptr };

		MotionController m_controller;
		Ref<View> m_views[MotionCtl_Num];
		LayoutRect m_iconRect;
	};
}

#endif