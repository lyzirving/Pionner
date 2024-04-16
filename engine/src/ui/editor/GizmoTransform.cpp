#include "GizmoTransform.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"

#include "gfx/renderer/Renderer.h"

#include "core/interface/hittable/HittableBox.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GizmoTransform"

#define GIZMO_TRANSM_OFFSET (0.06f)

namespace pio
{
	GizmoTransform::GizmoTransform() : EditorUI(), Hittable()
	{
		onCreateMesh();
	}

	void GizmoTransform::onCreateMesh()
	{
		Ref<Arrow3D> arrow = RefCast<MeshSource, Arrow3D>(MeshFactory::CreateArrow3D(0.013f, 0.4f, 0.025f, 0.1f, 24));
		m_arrow = RefCast<Asset, StaticMesh>(AssetsManager::CreateRuntimeAssets<StaticMesh>(arrow));

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

	void GizmoTransform::onDraw(const DrawParam &param)
	{
		Ref<UniformBufferSet> ubs = param.Ubs;
		auto drawFunc = [ubs](Ref<StaticMesh> mesh, Ref<HittableShape> shape, const glm::vec4 &color) mutable
		{		
			Renderer::SubmitRC([mesh, ubs, shape, color]() mutable
			{
				RenderState state{ Blend::Disable(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable(), RenderMode::MaterialPreview };
				auto arrow = RefCast<MeshSource, Arrow3D>(mesh->getMeshSource());
				arrow->setColor(color);

				Renderer::RenderSubmesh(mesh->getHandle(), 0, mesh->getMaterialTable(), Ref<RenderPass>(), ubs,
				                        shape->getTransform() * shape->getLocalTransform(), state);
			});
		};

		drawFunc(m_arrow, m_shape[EditorAxis_X], glm::vec4(1.f, 0.f, 0.f, 1.f));
		drawFunc(m_arrow, m_shape[EditorAxis_Y], glm::vec4(0.f, 1.f, 0.f, 1.f));
		drawFunc(m_arrow, m_shape[EditorAxis_Z], glm::vec4(0.f, 0.f, 1.f, 1.f));

		// draw outline of each gizmo
		for (uint32_t i = 0; i < EditorAxis_Cnt; i++)
		{
			if (m_shape[i])
				m_shape[i]->onDraw(param);
		}
	}

	bool GizmoTransform::onHit(HitQuery &query)
	{
		if (m_shape[EditorAxis_X]->onHit(query) || 
			m_shape[EditorAxis_Y]->onHit(query) ||
			m_shape[EditorAxis_Z]->onHit(query))
			return true;

		return false;
	}

	void GizmoTransform::setTranslation(const glm::vec3 &translation)
	{
		for (uint32_t i = 0; i < EditorAxis_Cnt; i++)
		{
			if (m_shape[i])
				m_shape[i]->setTranslation(translation);
		}
	}
}