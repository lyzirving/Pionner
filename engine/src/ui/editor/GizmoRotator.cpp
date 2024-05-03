#include "GizmoRotator.h"

#include "asset/AssetsManager.h"

#include "gfx/struct/Mesh.h"
#include "gfx/struct/Geometry.h"
#include "gfx/struct/Geometry2D.h"
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

		m_pendingVertex.reserve(2);
		m_direction = AssetsManager::CreateRuntimeAssets<LineSegment>("Rotate Direction");
		m_direction->Color = glm::vec4(1.f);
		m_direction->Vertex.reserve(2);
		m_direction->Indices.reserve(2);

		m_direction->Indices.push_back(0);
		m_direction->Indices.push_back(1);

		m_direction->VertexBuffer = VertexBuffer::Create(2 * sizeof(SimpleVertex));
		m_direction->VertexBuffer->setLayout(VertexBuffer::To<SimpleVertex>());

		m_direction->IndexBuffer = IndexBuffer::Create(m_direction->Indices.data(), m_direction->Indices.size() * sizeof(uint32_t), m_direction->Indices.size());

		m_direction->VertexArray = VertexArray::Create();
		m_direction->VertexArray->addVertexBuffer(m_direction->VertexBuffer);
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

		if (!m_pendingVertex.empty())
		{
			m_direction->Vertex.clear();
			m_direction->Vertex.assign(m_pendingVertex.begin(), m_pendingVertex.end());

			Ref<LineSegment> mesh = m_direction;
			glm::vec4 color = (m_selectedAxis == EditorAxis_X) ? glm::vec4(1.f, 0.f, 0.f, 1.f) : ((m_selectedAxis == EditorAxis_Y) ? glm::vec4(0.f, 1.f, 0.f, 1.f) : glm::vec4(0.f, 0.f, 1.f, 1.f));
				
			Renderer::SubmitRC([ubs, mesh, color]() mutable
			{
				RenderState state{ Blend::Common(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable(), RenderMode::MaterialPreview };
				mesh->VertexBuffer->setData(mesh->Vertex.data(), mesh->Vertex.size() * sizeof(SimpleVertex));
				mesh->Color = color;
				Renderer::RenderLineSegment(mesh->getHandle(), ubs, glm::mat4(1.f), state);
			});
		}
	}

	bool GizmoRotator::onHit(HitQuery &query)
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

		setSelectedAxis(EditorAxis_Num);
		return false;
	}

	bool GizmoRotator::onMouseButtonPressed(Event &event)
	{
		if (!bVisible()) return false;

		auto *e = event.as<MouseButtonPressedEvent>();
		glm::ivec2 vpPoint = ScreenToViewport(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam);
		HitQuery query(Ray::BuildFromScreen(vpPoint, m_cameraEnt->getComponent<CameraComponent>().Camera));

		if (onHit(query)) { m_lastHitPt = query.HitPt; }
		//if (query.Hit) LOGD("Press");
		return query.Hit;
	}

	bool GizmoRotator::onMouseButtonReleased(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;
		//LOGD("release");
		cancelSelection();
		m_angleDiff = m_rotatedAngle = 0.f;
		m_pendingVertex.clear();
		return true;
	}

	bool GizmoRotator::onMouseMoved(Event &event)
	{
		if (!bVisible() || !bSelected()) return false;

		auto *e = event.as<MouseMovedEvent>();
		auto *shape = m_shape[m_selectedAxis]->as<HittableFlatCircle>();
		const Plane &plane = shape->getPlane();

		glm::ivec2 vpPoint = ScreenToViewport(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam);
		HitQuery query(Ray::BuildFromScreen(vpPoint, m_cameraEnt->getComponent<CameraComponent>().Camera));

		if (Intersection(query, plane))
		{			
			glm::vec3 n = plane.getNormal();
			if (m_selectedAxis == EditorAxis_Y) { n = -n; }
			glm::vec3 origin = shape->getOrigin();

			glm::vec3 edge0 = glm::normalize(m_lastHitPt - origin);
			glm::vec3 edge1 = glm::normalize(query.HitPt - origin);

			float dot = glm::dot(edge0, edge1);
			glm::vec3 cross = glm::cross(edge0, edge1);
			float rotation{ 0.f };
			if (glm::dot(cross, n) > 0.f)
			{
				rotation = glm::degrees(std::acos(dot));
			}
			else
			{
				rotation = -glm::degrees(std::acos(dot));
			}
			m_rotatedAngle += rotation;
			m_angleDiff = rotation;
			//LOGD("rotated angle[%f], axis[%s]", m_rotatedAngle, EditorAxisStr(m_selectedAxis));
			m_lastHitPt = query.HitPt;
			m_pendingVertex.clear();
			m_pendingVertex.emplace_back(origin, glm::vec2(0.f));
			m_pendingVertex.emplace_back(query.HitPt, glm::vec2(0.f));
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
		std::for_each(std::begin(m_shape), std::end(m_shape), [x, y, z](const Ref<HittableShape> &s) { s->setTranslation(glm::vec3(x, y, z)); });
	}

	void GizmoRotator::setTranslation(const glm::vec3 &location)
	{
		std::for_each(std::begin(m_shape), std::end(m_shape), [location](const Ref<HittableShape> &s) { s->setTranslation(location); });
	}
}