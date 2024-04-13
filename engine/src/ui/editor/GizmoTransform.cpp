#include "GizmoTransform.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"

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

		m_localTransform[EditorAxis_X] = glm::translate(glm::mat4(1.f), glm::vec3(GIZMO_TRANSM_OFFSET, 0.f, 0.f)) * 
			                             glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
		m_localTransform[EditorAxis_Y] = glm::translate(glm::mat4(1.f), glm::vec3(0.f, GIZMO_TRANSM_OFFSET, 0.f));
		m_localTransform[EditorAxis_Z] = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, GIZMO_TRANSM_OFFSET)) *
			                             glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	}

	void GizmoTransform::onDraw()
	{
	}

	bool GizmoTransform::onHit(const HitResult result)
	{
		return false;
	}
}