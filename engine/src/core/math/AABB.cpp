#include "AABB.h"

#include "asset/AssetsManager.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include "gfx/renderer/Renderer.h"

static uint32_t g_AABBCnt{ 0 };

namespace pio
{
	AABB::AABB()
	{
		onCreateMesh();
	}

	AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : Min(min), Max(max)
	{
		onCreateMesh();
	}

	void AABB::onCreateMesh()
	{
		if (!m_outline)
		{
			m_outline = AssetsManager::CreateRuntimeAssets<LineMesh>(std::string("AABB-") + std::to_string(g_AABBCnt++));
			m_outline->Vertex.reserve(8);
			m_outline->Indices.reserve(2 * 12);

			m_outline->Vertex.emplace_back(m_points[0], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[1], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[2], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[3], Renderer::GetConfig().OutlineColor);

			m_outline->Vertex.emplace_back(m_points[4], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[5], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[6], Renderer::GetConfig().OutlineColor);
			m_outline->Vertex.emplace_back(m_points[7], Renderer::GetConfig().OutlineColor);

			// -------- bottom face -------- 
			m_outline->Indices.push_back(0);
			m_outline->Indices.push_back(1);

			m_outline->Indices.push_back(1);
			m_outline->Indices.push_back(2);

			m_outline->Indices.push_back(2);
			m_outline->Indices.push_back(3);

			m_outline->Indices.push_back(3);
			m_outline->Indices.push_back(0);

			// -------- side line -------- 
			m_outline->Indices.push_back(0);
			m_outline->Indices.push_back(4);

			m_outline->Indices.push_back(1);
			m_outline->Indices.push_back(5);

			m_outline->Indices.push_back(2);
			m_outline->Indices.push_back(6);

			m_outline->Indices.push_back(3);
			m_outline->Indices.push_back(7);

			// -------- top face -------- 
			m_outline->Indices.push_back(4);
			m_outline->Indices.push_back(5);

			m_outline->Indices.push_back(5);
			m_outline->Indices.push_back(6);

			m_outline->Indices.push_back(6);
			m_outline->Indices.push_back(7);

			m_outline->Indices.push_back(7);
			m_outline->Indices.push_back(4);

			m_outline->VertexBuffer = VertexBuffer::Create(m_outline->Vertex.data(), m_outline->Vertex.size() * sizeof(LineVertex));
			m_outline->VertexBuffer->setLayout(VertexBuffer::To<LineVertex>());

			m_outline->IndexBuffer = IndexBuffer::Create(m_outline->Indices.data(), m_outline->Indices.size() * sizeof(uint32_t), m_outline->Indices.size());

			m_outline->VertexArray = VertexArray::Create();
			m_outline->VertexArray->addVertexBuffer(m_outline->VertexBuffer);
		}
	}

	void AABB::onDraw(const DrawParam &param)
	{
		updatePoints();

		m_outline->Vertex.clear();
		m_outline->Vertex.reserve(8);
		m_outline->Vertex.emplace_back(m_points[0], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[1], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[2], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[3], Renderer::GetConfig().OutlineColor);

		m_outline->Vertex.emplace_back(m_points[4], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[5], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[6], Renderer::GetConfig().OutlineColor);
		m_outline->Vertex.emplace_back(m_points[7], Renderer::GetConfig().OutlineColor);

		Ref<LineMesh> mesh = m_outline;

		Renderer::SubmitTask([mesh]()
		{
			mesh->VertexBuffer->setData(mesh->Vertex.data(), mesh->Vertex.size() * sizeof(LineVertex));
		});

		Renderer::SubmitRC([param, mesh]() mutable
		{
			Ref<UniformBufferSet> ubs = param.UBSet;
			RenderState state{ Blend::Disable(), DepthTest::Disable(), CullFace::Disable(), StencilTest::Disable() };
			Renderer::RenderLine(mesh->getHandle(), ubs, glm::mat4(1.f), state);
		});
	}
}