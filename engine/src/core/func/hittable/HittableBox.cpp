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
	}

	bool HittableBox::onHit(HitQuery &query)
	{
		update();
		return Intersection(query, m_AABB);
	}

	void HittableBox::onDraw(const DrawParam &param)
	{
		//TODO: draw outline of HittableBox
	}

	void HittableBox::update()
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

	template<>
	bool HittableShape::is<HittableBox>() { return this->getShapeType() == HitShapeType::Box; }
}