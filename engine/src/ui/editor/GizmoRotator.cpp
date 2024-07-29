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

		m_pendingVertex.reserve(2);
		m_direction = AssetsManager::CreateRuntimeAssets<LineSegment>("Rotate Direction");		
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

		if (bSelected())
		{
			drawTorus(m_halfTorus, m_shape[m_selectedAxis], glm::vec3(1.f, 1.f, 1.f));
		}
		else
		{
			for (uint8_t i = 0; i < EditorAxis_Num; i++)
			{
				drawTorus(m_halfTorus, m_shape[i], GetAxisColor(EditorAxis(i), m_hoveringAxis));
			}
		}

		if (!m_pendingVertex.empty())
		{
			m_direction->Vertex.clear();
			m_direction->Vertex.assign(m_pendingVertex.begin(), m_pendingVertex.end());

			Ref<LineSegment> mesh = m_direction;
			glm::vec4 color = GetAxisColor(m_selectedAxis, EditorAxis_Num);

			Renderer::SubmitRC([ubs, mesh, color]() mutable
			{
				RenderState state{ Blend::Common(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable(), RenderMode::MaterialPreview };
				mesh->VertexBuffer->setData(mesh->Vertex.data(), mesh->Vertex.size() * sizeof(SimpleVertex));		
				Renderer::RenderLineSegment(mesh->getHandle(), color, ubs, glm::mat4(1.f), state);
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
		return false;
	}

	bool GizmoRotator::onMouseButtonPressed(Event& event)
	{
		if (!bVisible()) return false;

		auto* e = event.as<MouseButtonPressedEvent>();
		HitQuery query(Camera::Main->screenPointToRay(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam, true));

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
		if (!bVisible()) return false;

		cancelHovering();

		if (bSelected())
		{
			auto *e = event.as<MouseMovedEvent>();
			auto *shape = m_shape[m_selectedAxis]->as<HittableFlatCircle>();
			const Plane &plane = shape->getPlane();
			
			HitQuery query(Camera::Main->screenPointToRay(glm::vec2(e->getCursorX(), e->getCursorY()), m_layoutParam));
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
		
		return false;
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

	bool GizmoRotator::onMouseMoveHovering(Event &event)
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