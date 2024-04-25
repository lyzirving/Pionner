#include "GizmoRotator.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/MeshFactory.h"
#include "gfx/renderer/Renderer.h"

#include "scene/Registry.h"
#include "scene/Components.h"

#include "core/math/Intersection.h"
#include "core/func/hittable/HittableFlatCircle.h"

#include "ui/UiDef.h"
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
		m_cameraEnt = Registry::Get()->mainCameraEnt();
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

		m_shape[EditorAxis_X]->setLocalRotation(EulerAngle(90.f, 0.f, -90.f));
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

		auto *e = event.as<MouseButtonPressedEvent>();
		HitQuery query = intersectionWith(glm::vec2(e->getCursorX(), e->getCursorY()), m_shape[m_selectedAxis]->as<HittableFlatCircle>());

		m_mousePressed = query.Hit;
		if (m_mousePressed) { m_downPt = query.HitPt; }

		return m_mousePressed;
	}

	bool GizmoRotator::onMouseButtonReleased(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		bool consume = m_mousePressed;
		m_mousePressed = false;
		return consume;
	}

	bool GizmoRotator::onMouseMoved(Event &event)
	{
		if (!bVisible() || !bSelected() || !m_mousePressed) return false;

		auto *e = event.as<MouseMovedEvent>();
		HitQuery query = intersectionWith(glm::vec2(e->getCursorX(), e->getCursorY()), m_shape[m_selectedAxis]->as<HittableFlatCircle>());
		if (query.Hit)
		{			
			auto *shape = m_shape[m_selectedAxis]->as<HittableFlatCircle>();
			const auto &plane = shape->getPlane();
			glm::vec3 n = plane.getNormal();
			if (m_selectedAxis == EditorAxis_Z) { n = -n; }
			glm::vec3 origin = shape->getOrigin();

			glm::vec3 edge0 = m_downPt - origin;
			glm::vec3 edge1 = query.HitPt - origin;

			float dot = glm::dot(edge0, edge1);
			glm::vec3 cross = glm::cross(edge0, edge1);
			float rotation{ 0.f };
			if (glm::dot(cross, n) > 0.f)
			{
				rotation = glm::degrees(std::acos(dot));
			}
			else
			{
				rotation = 360.f - glm::degrees(std::acos(dot));
			}
			LOGD("%s rotation[%f]", EditorUI::EditorAxisStr(m_selectedAxis), rotation);
		}
		else
		{
			LOGE("invalid opt");
		}

		return true;
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

	HitQuery GizmoRotator::intersectionWith(const glm::vec2 &cursor, HittableFlatCircle *shape)
	{
		glm::ivec2 vpPoint = UiDef::ScreenToViewport(cursor, m_layoutParam);
		HitQuery query(Ray::BuildFromScreen(vpPoint, m_cameraEnt->getComponent<CameraComponent>().Camera));

		shape->update();
		const Plane &plane = shape->getPlane();
		Intersection(query, plane);
		return query;
	}
}