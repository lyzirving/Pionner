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
	class PhysicsScene;

	class C3DControlLayer : public Layer
	{
	public:
		C3DControlLayer(const LayoutParams &param);
		virtual ~C3DControlLayer() = default;

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
		void onDrawSelectionCtl(const Timestep &ts);

		void onSelectionMoved(Ref<Entity> &selection, PhysicsActor *ctlActor, const glm::vec2 &cursor, const glm::vec2 &last, const LayoutParams &param);
		void onIntersect(const HitResult &result);

	private:
		bool m_drawCircle{ false };
		LayoutParams m_circleLayoutParam;

		Ref<Entity> m_mainCameraEnt, m_sceneEnt;

		Ref<UniformBufferSet> m_visionUBSet;
		CameraUD m_visionCamUD;

		Ref<UniformBufferSet> m_selectionUBSet;
		CameraUD m_selectionCamUD;

		Ref<UiCoordinate3D> m_visionCoord;
		Ref<UiCoordinate3D> m_selectCoord;

		Ref<PhysicsScene> m_world;
		bool m_objSelectd{ false };
		PhysicsActor *m_hitCtlActor{ nullptr };
	};
}

#endif