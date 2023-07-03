#include <cmath>

#include "core/math/AABB.h"

namespace pio
{
	AABB::AABB() : m_aa(0.f), m_bb(0.f) {}

	AABB::AABB(const glm::vec3& aa, const glm::vec3& bb) : m_aa(0.f), m_bb(0.f)
	{
		m_aa = glm::min(aa, bb);
		m_bb = glm::max(aa, bb);
	}

	AABB::~AABB() = default;

	bool AABB::contains(const glm::vec3& pt) const
	{
		return pt.x >= m_aa.x && pt.y >= m_aa.y && pt.z >= m_aa.z &&
			   pt.x <= m_bb.x && pt.y <= m_bb.y && pt.z <= m_bb.z;
	}

	AABB AABB::surroundingBox(const AABB& lhs, const AABB& rhs)
	{
		glm::vec3 minPos = glm::min(lhs.m_aa, rhs.m_aa);
		glm::vec3 maxPos = glm::max(lhs.m_bb, rhs.m_bb);
		return AABB(minPos, maxPos);
	}
}