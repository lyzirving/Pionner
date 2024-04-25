#include "GizmoRotator.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/renderer/Renderer.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "core/func/hittable/HittableFlatCircle.h"
#include "window/event/MouseEvent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GizmoRotator"

#define HALF_CIRCLE_ITR (36)

namespace pio
{
	GizmoRotator::GizmoRotator() : EditorUI(), Hittable() 
	{
		onCreateMesh();
	}

	void GizmoRotator::onCreateMesh()
	{
		Ref<MeshSource> meshSrc = MeshFactory::CreateTorus(m_radius, m_ringWidth, glm::vec3(1.f, 0.f, 0.f), 36, 16, true);
		meshSrc->as<Geometry>()->setAlpha(0.6f);
		m_halfTorus = AssetsManager::CreateRuntimeAssets<StaticMesh>(meshSrc);

		m_shape[EditorAxis_X] = CreateRef<HittableFlatCircle>(m_radius);
		m_shape[EditorAxis_Y] = CreateRef<HittableFlatCircle>(m_radius);
		m_shape[EditorAxis_Z] = CreateRef<HittableFlatCircle>(m_radius);

		m_shape[EditorAxis_X]->setLocalRotation(EulerAngle(90.f, 0.f, 90.f));
		m_shape[EditorAxis_Y]->setLocalRotation(EulerAngle(90.f, 0.f, 0.f));
	}

	void GizmoRotator::onDraw(const DrawParam &param)
	{
		if (!bVisible()) return;

		Ref<UniformBufferSet> ubs = param.UBSet;

		auto drawTorus = [ubs](Ref<StaticMesh> mesh, Ref<HittableShape> shape, const glm::vec3 &color)
		{
			Renderer::SubmitRC([ubs, mesh, shape, color]() mutable
			{
				Ref<MeshSource> meshSrc = mesh->getMeshSource();
				meshSrc->as<Torus>()->setColor(color);
			    RenderState state{ Blend::Common(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable(), RenderMode::MaterialPreview };
				Renderer::RenderSubmesh(mesh->getHandle(), 0, mesh->getMaterialTable(), Ref<RenderPass>(), ubs, shape->getTransform() * shape->getLocalTransform(), state);
			});
		};

		drawTorus(m_halfTorus, m_shape[EditorAxis_X], glm::vec3(1.f, 0.f, 0.f));
		drawTorus(m_halfTorus, m_shape[EditorAxis_Y], glm::vec3(0.f, 1.f, 0.f));
		drawTorus(m_halfTorus, m_shape[EditorAxis_Z], glm::vec3(0.f, 0.f, 1.f));
	}

	bool GizmoRotator::onHit(HitQuery &query)
	{
		if (!bVisible()) return false;
		if (m_shape[EditorAxis_X]->onHit(query))
		{
			//LOGD("X axis hit");
			query.HitActor = m_shape[EditorAxis_X].get();
			setSelectedAxis(EditorAxis_X);
			return true;
		}

		if (m_shape[EditorAxis_Y]->onHit(query))
		{
			//LOGD("Y axis hit");
			query.HitActor = m_shape[EditorAxis_Y].get();
			setSelectedAxis(EditorAxis_Y);
			return true;
		}

		if (m_shape[EditorAxis_Z]->onHit(query))
		{
			//LOGD("Z axis hit");
			query.HitActor = m_shape[EditorAxis_Z].get();
			setSelectedAxis(EditorAxis_Z);
			return true;
		}
		setSelectedAxis(EditorAxis_Num);
		return false;
	}

	bool GizmoRotator::onMouseButtonPressed(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		return false;
	}

	bool GizmoRotator::onMouseButtonReleased(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		return false;
	}

	bool GizmoRotator::onMouseMoved(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		return false;
	}

	bool GizmoRotator::onMouseScrolled(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		return false;
	}

	void GizmoRotator::setTranslation(float x, float y, float z)
	{
		for (uint32_t i = 0; i < EditorAxis_Num; i++)
		{
			if (m_shape[i])
				m_shape[i]->setTranslation(glm::vec3(x, y, z));
		}
	}
}