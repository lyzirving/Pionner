#include "GizmoTransform.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/renderer/Renderer.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "core/math/Intersection.h"
#include "core/func/hittable/HittableBox.h"
#include "window/event/MouseEvent.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GizmoTransform"

#define GIZMO_TRANSM_OFFSET (0.06f)
#define GIZMO_TRANSM_SCALE  (100.f)

namespace pio
{
	GizmoTransform::GizmoTransform() : EditorUI(), Hittable()
	{
		onCreateMesh();		
	}

	void GizmoTransform::onCreateMesh()
	{
		// [TODO] make attributes supported by interface of configuration
		float cylinderRadius = 0.013f;
		float cylinderHeight = 0.4f;
		float coneRadius     = 0.025f;
		float coneHeight     = 0.1f;

		Ref<Arrow3D> arrow = RefCast<MeshSource, Arrow3D>(MeshFactory::CreateArrow3D(0.013f, 0.4f, 0.025f, 0.1f, 24));
		m_arrow = AssetsManager::CreateRuntimeAssets<StaticMesh>(arrow);

		float size = std::max(cylinderRadius, coneRadius) * 2.f;
		glm::vec3 len = glm::vec3(size, cylinderHeight + coneHeight, size);
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
		if (!bVisible()) return;

		Ref<UniformBufferSet> ubs = param.UBSet;
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

		if (bSelected())
		{
			drawFunc(m_arrow, m_shape[m_selectedAxis], glm::vec4(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			for (uint8_t i = 0; i < EditorAxis_Num; i++)
			{
				drawFunc(m_arrow, m_shape[i], GetAxisColor(EditorAxis(i), m_hoveringAxis));
			}
		}

		if (bShowOutline())
		{
			for (uint32_t i = 0; i < EditorAxis_Num; i++)
				if (m_shape[i])
					m_shape[i]->onDraw(param);
		}
	}

	bool GizmoTransform::onHit(HitQuery &query)
	{
		if (!bVisible()) return false;

		if (m_shape[EditorAxis_X]->onHit(query))
		{
			//LOGD("X axis hit");
			setSelectedAxis(EditorAxis_X);
			return true;
		}

		if (m_shape[EditorAxis_Y]->onHit(query))
		{
			//LOGD("Y axis hit");		
			setSelectedAxis(EditorAxis_Y);
			return true;
		}

		if (m_shape[EditorAxis_Z]->onHit(query))
		{
			//LOGD("Z axis hit");		
			setSelectedAxis(EditorAxis_Z);
			return true;
		}
		return false;		
	}

	bool GizmoTransform::onMouseButtonPressed(Event &event)
	{
		if (!bVisible()) return false;

		auto *e = event.as<MouseButtonPressedEvent>();		
		HitQuery query(Camera::Main->screenPointToRay(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam, true));
		
		if (onHit(query)) { m_lastHitPt = query.R.PtOnNear; }
		return query.Hit;
	}

	bool GizmoTransform::onMouseButtonReleased(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		//LOGD("release");
		cancelSelection();
		m_transferDist = m_transferDiff = m_transferVec = glm::vec3(0.f);		
		return true;
	}

	bool GizmoTransform::onMouseMoved(Event& event)
	{
		if (!bVisible()) return false;

		cancelHovering();

		if (bSelected())
		{
			auto* e = event.as<MouseMovedEvent>();
			glm::vec3 ptOnNearPlane = Camera::Main->screenPointToNearPlane(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam);
			glm::vec3 diff = ptOnNearPlane - m_lastHitPt;
			m_transferVec += diff;
			m_transferDiff = EditorUI::GetAxis(m_selectedAxis) * glm::dot(diff, EditorUI::GetAxis(m_selectedAxis)) * GIZMO_TRANSM_SCALE;
			m_transferDist = EditorUI::GetAxis(m_selectedAxis) * glm::dot(m_transferVec, EditorUI::GetAxis(m_selectedAxis)) * GIZMO_TRANSM_SCALE;
			m_lastHitPt = ptOnNearPlane;
			//LOGD("transfer[%f], diff[%f, %f, %f], axis[%s]", m_transferDist, diff.x, diff.y, diff.z, EditorAxisStr(m_selectedAxis));
			return true;
		}

		return false;
	}

	bool GizmoTransform::onMouseScrolled(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		return false;
	}

	void GizmoTransform::setTranslation(float x, float y, float z)
	{
		std::for_each(std::begin(m_shape), std::end(m_shape), [x, y, z](const Ref<HittableShape> &s) { s->setTranslation(glm::vec3(x, y, z)); });
	}

	void GizmoTransform::setTranslation(const glm::vec3 &translation)
	{
		std::for_each(std::begin(m_shape), std::end(m_shape), [translation](const Ref<HittableShape> &s) { s->setTranslation(translation); });
	}

	bool GizmoTransform::onMouseMoveHovering(Event &event)
	{
		if (!bVisible()) return false;

		cancelHovering();

		auto *e = event.as<MouseMovedEvent>();		
		HitQuery query(Camera::Main->screenPointToRay(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam));
		
		if (m_shape[EditorAxis_X]->onHit(query))
		{
			//LOGD("hovering on X axis");
			setHoveringAxis(EditorAxis_X);
			return true;
		}

		if (m_shape[EditorAxis_Y]->onHit(query))
		{
			//LOGD("hovering on Y axis");
			setHoveringAxis(EditorAxis_Y);
			return true;
		}

		if (m_shape[EditorAxis_Z]->onHit(query))
		{
			//LOGD("hovering on Z axis");
			setHoveringAxis(EditorAxis_Z);
			return true;
		}

		return false;
	}
}