#include "HittableBox.h"

#include "asset/AssetsManager.h"
#include "core/math/Intersection.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "HittableBox"

namespace pio
{
	static uint32_t g_HittableBoxCnt{ 0 };

	HittableBox::HittableBox() : HittableShape()
	{
		onCreateShape();
	}

	HittableBox::HittableBox(const glm::vec3 &len) : HittableShape(), m_len(glm::abs(len))
	{
		onCreateShape();
	}

	void HittableBox::onCreateShape()
	{		
		/*
		* outline vertex for HittableBox, O(0.f, 0.f) is origin of local coordinate
		*       v7 ----------- v6
		*      /|            / |
		*     / |           /  |
		*   v4 -|--------- v5  |
		*    |  v3 --------|-- v2
		*    | /           |  /
		*    |/      O     | /
		*    /             |/
		*    v0 ---------- v1
		*/
		m_points[0] = glm::vec3(-0.5f * m_len.x, 0.f, 0.5f * m_len.z);
		m_points[1] = glm::vec3(0.5f * m_len.x, 0.f, 0.5f * m_len.z);
		m_points[2] = glm::vec3(0.5f * m_len.x, 0.f, -0.5f * m_len.z);
		m_points[3] = glm::vec3(-0.5f * m_len.x, 0.f, -0.5f * m_len.z);

		m_points[4] = glm::vec3(-0.5f * m_len.x, m_len.y, 0.5f * m_len.z);
		m_points[5] = glm::vec3(0.5f * m_len.x, m_len.y, 0.5f * m_len.z);
		m_points[6] = glm::vec3(0.5f * m_len.x, m_len.y, -0.5f * m_len.z);
		m_points[7] = glm::vec3(-0.5f * m_len.x, m_len.y, -0.5f * m_len.z);	

		// save original AABB
		for (uint32_t i = 0; i < 8; i++)
		{
			m_originalAABB.Min = glm::min(m_originalAABB.Min, m_points[i]);
			m_originalAABB.Max = glm::max(m_originalAABB.Max, m_points[i]);
		}

		m_outline = AssetsManager::CreateRuntimeAssets<LineMesh>(std::string("HittaleBox-") + std::to_string(g_HittableBoxCnt++));
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

	bool HittableBox::onHit(HitQuery &query)
	{
		updateAABB();
		return Intersection(query, m_AABB);
	}

	void HittableBox::onDraw(const DrawParam &param)
	{
		HittableBox *self = this;
		Ref<LineMesh> mesh = m_outline;		
		Renderer::SubmitRC([param, self, mesh]() mutable
		{
			glm::mat4 trans = self->getTransform() * self->getLocalTransform();
			Ref<UniformBufferSet> ubs = param.UBSet;
			RenderState state{ Blend::Disable(), DepthTest::Disable(), CullFace::Disable(), StencilTest::Disable() };
			Renderer::RenderLine(mesh->getHandle(), ubs, trans, state);
		});
	}

	void HittableBox::updateAABB()
	{
		if (!bTransformChange())
			return;

		invalidTransform(false);

		glm::mat4 mat = m_transform.mat() * m_localTransform.mat();
		glm::vec3 p0 = mat * glm::vec4(m_originalAABB.Min, 1.f);
		glm::vec3 p1 = mat * glm::vec4(m_originalAABB.Max, 1.f);
		m_AABB.Min = glm::min(p0, p1);
		m_AABB.Max = glm::max(p0, p1);
	}
}