#include "GizmoTransform.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"

#include "core/interface/hittable/HittableBox.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GizmoTransform"

#define GIZMO_TRANSM_OFFSET (0.06f)

namespace pio
{
	void GizmoTransform::onCreateMesh()
	{
		Ref<MeshSource> meshSource = MeshFactory::CreateArrow3D(0.013f, 0.4f, 0.025f, 0.1f, 24);
		Ref<Asset> meshAsset = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSource);
		m_arrow = RefCast<MeshSource, Arrow3D>(meshSource);

		glm::vec3 len = glm::vec3(0.2f, 1.f, 0.2f);
		m_shape[EditorAxis_X] = CreateRef<HittableBox>(len);
		m_shape[EditorAxis_Y] = CreateRef<HittableBox>(len);
		m_shape[EditorAxis_Z] = CreateRef<HittableBox>(len);

		m_shape[EditorAxis_X]->setLocalTranslation(glm::vec3(GIZMO_TRANSM_OFFSET, 0.f, 0.f));
		m_shape[EditorAxis_X]->setLocalRotation(EulerAngle(0.f, 0.f, -90.f));

		m_shape[EditorAxis_Y]->setLocalTranslation(glm::vec3(0.f, GIZMO_TRANSM_OFFSET, 0.f));	

		m_shape[EditorAxis_Z]->setLocalTranslation(glm::vec3(0.f, 0.f, GIZMO_TRANSM_OFFSET));
		m_shape[EditorAxis_Z]->setLocalRotation(EulerAngle(90.f, 0.f, 0.f));
	}

	void GizmoTransform::onDraw()
	{
	}

	bool GizmoTransform::onHit(HitQuery &query)
	{
		if (m_shape[EditorAxis_X]->onHit(query))
			return true;

		if (m_shape[EditorAxis_Y]->onHit(query))
			return true;

		if (m_shape[EditorAxis_Z]->onHit(query))
			return true;

		return false;
	}
}