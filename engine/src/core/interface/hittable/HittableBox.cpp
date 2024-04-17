#include "HittableBox.h"

#include "asset/AssetsManager.h"

#include "gfx/rhi/VertexArray.h"
#include "gfx/rhi/VertexBuffer.h"
#include "gfx/rhi/IndexBuffer.h"

#include "gfx/renderer/Renderer.h"

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

		m_outline = AssetsManager::CreateRuntimeAssets<LineMesh>(std::string("HittaleBox") + std::to_string(g_HittableBoxCnt++));
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

		updateAABB();
	}

	bool HittableBox::onHit(HitQuery &query)
	{
		updateAABB();
		return false;
	#if 0
		const AABB &aabb = m_aabb;

		const Ray &r = query.R;
		float t{ -1.f };
		glm::vec3 ptOnPlane{ 0.f };

		if (!Math::IsZero(r.Dir.x))
		{
			//ray: P=O+D*t
			if (r.Dir.x > 0)
				t = (aabb.Min.x - r.Origin.x) / r.Dir.x;
			else
				t = (aabb.Max.x - r.Origin.x) / r.Dir.x;

			if (t > 0.f)
			{
				ptOnPlane = r.Origin + t * r.Dir;
				if (aabb.Min.y < ptOnPlane.y && ptOnPlane.y < aabb.Max.y &&
					aabb.Min.z < ptOnPlane.z && ptOnPlane.z < aabb.Max.z)
				{
					query.Hit = true;
					query.Intersection = ptOnPlane;
					return true;
				}
			}
		}

		if (!Math::IsZero(r.Dir.y))
		{
			if (r.Dir.y > 0)
				t = (aabb.Min.y - r.Origin.y) / r.Dir.y;
			else
				t = (aabb.Max.y - r.Origin.y) / r.Dir.y;

			if (t > 0.f)
			{
				ptOnPlane = r.Origin + t * r.Dir;
				if (aabb.Min.z < ptOnPlane.z && ptOnPlane.z < aabb.Max.z &&
					aabb.Min.x < ptOnPlane.x && ptOnPlane.x < aabb.Max.x)
				{
					query.Hit = true;
					query.Intersection = ptOnPlane;
					return true;
				}
			}
		}

		if (!Math::IsZero(r.Dir.z))
		{
			if (r.Dir.z > 0)
				t = (aabb.Min.z - r.Origin.z) / r.Dir.z;
			else
				t = (aabb.Max.z - r.Origin.z) / r.Dir.z;

			if (t > 0.f)
			{
				ptOnPlane = r.Origin + t * r.Dir;
				if (aabb.Min.x < ptOnPlane.x && ptOnPlane.x < aabb.Max.x &&
					aabb.Min.y < ptOnPlane.y && ptOnPlane.y < aabb.Max.y)
				{
					query.Hit = true;
					query.Intersection = ptOnPlane;
					return true;
				}
			}
		}

		return false;
	#endif // 0
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
		m_aabb.reset();
		for (uint32_t i = 0; i < 8; i++)
		{
			glm::vec3 p = m_transform.mat() * m_localTransform.mat() * glm::vec4(m_points[i], 1.f);
			m_aabb.Min = glm::min(m_aabb.Min, p);
			m_aabb.Max = glm::max(m_aabb.Max, p);
		}
	}
}