#ifndef __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__
#define __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__

#include "EditorUI.h"
#include "core/interface/hittable/Hittable.h"

namespace pio
{
	class Arrow3D;
	class HittableShape;

	class GizmoTransform : public EditorUI, public Hittable
	{
	public:
		GizmoTransform() {}
		~GizmoTransform() = default;

		virtual void onCreateMesh() override;
		virtual void onDraw() override;
		virtual bool onHit(HitQuery &query) override;

	private:
		Ref<Arrow3D> m_arrow;
		std::shared_ptr<HittableShape> m_shape[EditorAxis_Cnt];
	};
}

#endif