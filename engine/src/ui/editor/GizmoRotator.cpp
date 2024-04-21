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
		//createHalfCircle();

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

		/*auto drawLineSeg = [ubs](Ref<LineSegment> mesh, Ref<HittableShape> shape, const glm::vec4 &color) mutable
		{
			Renderer::SubmitRC([mesh, ubs, shape, color]() mutable
			{
				RenderState state{ Blend::Disable(), DepthTest::Disable(), CullFace::Common(), StencilTest::Disable(), RenderMode::MaterialPreview };
				mesh->Color = color;
				Renderer::RenderLineSegment(mesh->getHandle(), ubs, shape->getTransform() * shape->getLocalTransform(), state);
			});
		};

		drawLineSeg(m_halfCircle, m_shape[EditorAxis_X], glm::vec4(1.f, 1.f, 1.f, 1.f));
		drawLineSeg(m_halfCircle, m_shape[EditorAxis_Y], glm::vec4(1.f, 1.f, 1.f, 1.f));
		drawLineSeg(m_halfCircle, m_shape[EditorAxis_Z], glm::vec4(1.f, 1.f, 1.f, 1.f));*/

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
			m_selectedAxis = EditorAxis_X;
			return true;
		}
		if (m_shape[EditorAxis_Y]->onHit(query))
		{
			//LOGD("Y axis hit");
			query.HitActor = m_shape[EditorAxis_Y].get();
			m_selectedAxis = EditorAxis_Y;
			return true;
		}
		if (m_shape[EditorAxis_Z]->onHit(query))
		{
			//LOGD("Z axis hit");
			query.HitActor = m_shape[EditorAxis_Z].get();
			m_selectedAxis = EditorAxis_Z;
			return true;
		}
		m_selectedAxis = EditorAxis_Num;
		return false;
	}

	bool GizmoRotator::onMouseButtonPressed(Event &event)
	{
		if (!bVisible()) return false;
		return false;
	}

	bool GizmoRotator::onMouseButtonReleased(Event &event)
	{
		if (!bVisible()) return false;
		return false;
	}

	bool GizmoRotator::onMouseMoved(Event &event)
	{
		if (!bVisible()) return false;
		return false;
	}

	bool GizmoRotator::onMouseScrolled(Event &event)
	{
		if (!bVisible()) return false;
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

	void GizmoRotator::createHalfCircle()
	{
		m_halfCircle = AssetsManager::CreateRuntimeAssets<LineSegment>("GizmoRotator");
		m_halfCircle->Color = glm::vec4(1.f, 0.f, 0.f, 1.f);
		m_halfCircle->Vertex.reserve(HALF_CIRCLE_ITR);
		m_halfCircle->Indices.reserve((HALF_CIRCLE_ITR - 1) * 2);

		uint32_t i{ 0 };
		float step = 180.f / HALF_CIRCLE_ITR;
		for (; i < (HALF_CIRCLE_ITR - 1); ++i)
		{
			SimpleVertex v;
			v.Position = m_radius * glm::vec3(std::cos(glm::radians(float(i) * step)), std::sin(glm::radians(float(i) * step)), 0.f);
			m_halfCircle->Vertex.emplace_back(v);
			m_halfCircle->Indices.emplace_back(i);
			m_halfCircle->Indices.emplace_back(i + 1);
		}
		SimpleVertex v;
		v.Position = m_radius * glm::vec3(std::cos(glm::radians(float(i) * step)), std::sin(glm::radians(float(i) * step)), 0.f);
		m_halfCircle->Vertex.emplace_back(v);

		m_halfCircle->VertexBuffer = VertexBuffer::Create(m_halfCircle->Vertex.data(), m_halfCircle->Vertex.size() * sizeof(SimpleVertex));
		m_halfCircle->VertexBuffer->setLayout(VertexBuffer::To<SimpleVertex>());

		m_halfCircle->IndexBuffer = IndexBuffer::Create(m_halfCircle->Indices.data(), m_halfCircle->Indices.size() * sizeof(uint32_t), m_halfCircle->Indices.size());

		m_halfCircle->VertexArray = VertexArray::Create();
		m_halfCircle->VertexArray->addVertexBuffer(m_halfCircle->VertexBuffer);
	}
}