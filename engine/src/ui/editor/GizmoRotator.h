#ifndef __PIONNER_UI_EDITOR_GIZMO_ROTATOR_H__
#define __PIONNER_UI_EDITOR_GIZMO_ROTATOR_H__

#include "EditorUI.h"
#include "core/func/hittable/HittableShape.h"

namespace pio
{
	class LineSegment;
	class Entity;
	class StaticMesh;

	class GizmoRotator : public EditorUI, public Hittable
	{
	public:
		GizmoRotator();
		~GizmoRotator() = default;

		virtual void onCreateMesh() override;
		virtual void onDraw(const DrawParam &param) override;
		virtual bool onHit(HitQuery &query) override;

		virtual bool onMouseButtonPressed(Event &event) override;
		virtual bool onMouseButtonReleased(Event &event) override;
		virtual bool onMouseMoved(Event &event) override;
		virtual bool onMouseScrolled(Event &event) override;

		void setTranslation(float x, float y, float z);

	private:
		void createHalfCircle();

	private:
		float m_radius{ 0.5f }, m_ringWidth{ 0.015f };
		Ref<LineSegment>   m_halfCircle;
		Ref<StaticMesh>    m_halfTorus;
		Ref<HittableShape> m_shape[EditorAxis_Num];
		Ref<Entity>        m_cameraEnt;
		EditorAxis         m_selectedAxis{ EditorAxis_Num };
	};
}

#endif