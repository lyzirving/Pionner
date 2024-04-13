#ifndef __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__
#define __PIONNER_UI_EDITOR_GIZMO_TRANSFORM_H__

#include "EditorUI.h"
#include "core/hittable/Hittable.h"

namespace pio
{
	class Arrow3D;

	class GizmoTransform : public EditorUI, public Hittable
	{
	public:
		GizmoTransform() {}
		~GizmoTransform() = default;

		virtual void onCreateMesh() override;
		virtual void onDraw() override;
		virtual bool onHit(const HitResult result) override;

	private:
		Ref<Arrow3D> m_arrow;
		glm::mat4 m_localTransform[EditorAxis_Cnt] { glm::mat4(1.f), glm::mat4(1.f), glm::mat4(1.f) };
		std::shared_ptr<HittableShape> m_shape[EditorAxis_Cnt];
	};
}

#endif