#ifndef __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__
#define __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__

#include "EditorUI.h"
#include "core/interface/hittable/Hittable.h"

namespace pio
{
	class StaticMesh;
	class HittableShape;

	class GizmoTransform : public EditorUI, public Hittable
	{
	public:
		GizmoTransform();
		~GizmoTransform() = default;

		virtual void onCreateMesh() override;
		virtual void onDraw(const DrawParam &param) override;
		virtual bool onHit(HitQuery &query) override;

		void setTranslation(const glm::vec3 &translation);

	private:
		Ref<StaticMesh> m_arrow;
		Ref<HittableShape> m_shape[EditorAxis_Cnt];
	};
}

#endif