#ifndef __PIONNER_UI_MOTION_CONTROLLER_H__
#define __PIONNER_UI_MOTION_CONTROLLER_H__

#include "UiDef.h"

namespace pio
{
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

	class View;
	class Entity;
	class PhysicsActor;

	class MotionController
	{
		PIO_SINGLETON_DECLARE(MotionController)

	public:
		static bool bClick(uint64_t now, uint64_t pre) { return (now - pre) <= CLICK_INTERVAL; }
		
		static void CtlActor(PhysicsActor *ac) { MotionController::Get()->m_ctlActor = ac; }
		static void PressCtlActor(bool p = true) { MotionController::Get()->m_pressCtlAc = p; }
		static void DownTime(uint64_t t) { MotionController::Get()->m_downTime = t; }
		static void Mode(MotionCtlMode m) { MotionController::Get()->m_mode = m; }
		static void SelectView(const Ref<View> &view) { MotionController::Get()->m_selectedView = view; }
		static void SelectObj3D(const Ref<Entity> &obj) { MotionController::Get()->m_selectedObj3D = obj; }
		static void SelectSprite(const Ref<Entity> &sprite) { MotionController::Get()->m_selectedSprite = sprite; }
		static void SelectAxis(MotionCtlAxis axis) { MotionController::Get()->m_selectedAxis = axis; }
		static void Target(MotionTarget t) { MotionController::Get()->m_target = t; }
		static void WinCursor(const glm::vec2 &cursor) { MotionController::Get()->m_winCursor = cursor; }

		static bool bCtlAcPressed() { return MotionController::Get()->m_pressCtlAc; }
		static bool bMode(MotionCtlMode m) { return MotionController::Get()->m_mode == m; }
		static bool bTarget(MotionTarget t) { return MotionController::Get()->m_target == t; }
		static bool bObj3dSelectd() { return MotionController::Get()->m_selectedObj3D != nullptr; }
		static bool bSpriteSelectd() { return MotionController::Get()->m_selectedSprite != nullptr; }	
		static bool bSpriteSelectd(const Ref<Entity> &sprite) { return MotionController::Get()->m_selectedSprite == sprite; }
		static bool bSelectedView() { return MotionController::Get()->m_selectedView != nullptr; }
		static bool bSelectedView(const Ref<View> &v) { return MotionController::Get()->m_selectedView == v; }

		static uint64_t GetDownTime() { return MotionController::Get()->m_downTime; }
		static MotionCtlMode GetMode() { return MotionController::Get()->m_mode; }
		static MotionTarget GetTarget() { return MotionController::Get()->m_target; }
		static glm::vec2 GetWinCursor() { return MotionController::Get()->m_winCursor; }
		static Ref<Entity> GetObj3D() { return MotionController::Get()->m_selectedObj3D; }
		static Ref<Entity> GetSprite() { return MotionController::Get()->m_selectedSprite; }
		static Ref<View> GetView() { return MotionController::Get()->m_selectedView; }
		static MotionCtlAxis GetAixs() { return MotionController::Get()->m_selectedAxis; }
		static PhysicsActor *GetCtlAc() { return MotionController::Get()->m_ctlActor; }

	private:
		Ref<View> m_selectedView;
		Ref<Entity> m_selectedObj3D;
		Ref<Entity> m_selectedSprite;
		MotionCtlAxis m_selectedAxis{ MotionCtlAxis_Num };
		MotionCtlMode m_mode{ MotionCtl_Move };
		MotionTarget m_target{ MotionTarget_None };
		PhysicsActor *m_ctlActor{ nullptr };
		bool m_pressCtlAc{ false };

		uint64_t m_downTime{ 0 };// ms
		glm::vec2 m_winCursor{ -1.f };

	private:
		friend class MotionControlLayer;
	};
}

#endif