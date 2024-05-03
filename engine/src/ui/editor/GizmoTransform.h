#ifndef __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__
#define __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__

#include "EditorUI.h"
#include "core/func/hittable/HittableShape.h"

namespace pio
{
	class StaticMesh;
	class HittableShape;
	class Entity;

	class GizmoTransform : public EditorUI, public Hittable
	{
	public:
		GizmoTransform();
		~GizmoTransform() = default;

		virtual void onCreateMesh() override;
		virtual void onDraw(const DrawParam &param) override;
		virtual bool onHit(HitQuery &query) override;

		virtual bool onMouseButtonPressed(Event &event) override;
		virtual bool onMouseButtonReleased(Event &event) override;
		virtual bool onMouseMoved(Event &event) override;
		virtual bool onMouseScrolled(Event &event) override;

		void setTranslation(float x, float y, float z);
		void setTranslation(const glm::vec3 &translation);	

		const glm::vec3 &getTransferAxis() const { return GetAxis(m_selectedAxis); }
		glm::vec3 getTransferDist() const { return m_transferDist; }
		glm::vec3 getTransferDiff() const { return m_transferDiff; }

	private:
		bool onMouseMoveHovering(Event &event);

		void setSelectedAxis(EditorAxis axis) { m_selectedAxis = axis; }
		void cancelSelection() { setSelectedAxis(EditorAxis_Num); }
		void setHoveringAxis(EditorAxis axis) { m_hoveringAxis = axis; }
		void cancelHovering() { setHoveringAxis(EditorAxis_Num); }

		bool bSelected() const { return m_selectedAxis < EditorAxis_Num; };
		bool bHovering() const { return m_hoveringAxis < EditorAxis_Num; };
	
	private:
		Ref<StaticMesh> m_arrow;
		Ref<HittableShape> m_shape[EditorAxis_Num];
		Ref<Entity> m_cameraEnt;

		glm::vec3  m_lastHitPt{ 0.f }, m_transferVec{ 0.f }, m_transferDist{0.f}, m_transferDiff{ 0.f };
		EditorAxis m_selectedAxis{ EditorAxis_Num }, m_hoveringAxis{ EditorAxis_Num };
	};
}

#endif