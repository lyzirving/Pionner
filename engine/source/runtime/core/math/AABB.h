#ifndef __AABB_H__
#define __AABB_H__

#include <glm/glm.hpp>

namespace pio
{
	class AABB
	{
	public:
		AABB();
		AABB(const glm::vec3& aa, const glm::vec3& bb);
		~AABB();

		inline const glm::vec3 AA() const { return m_aa; }
		inline const glm::vec3 BB() const { return m_bb; }
		inline void setAA(const glm::vec3& aa) { m_aa = aa; }
		inline void setBB(const glm::vec3& bb) { m_bb = bb; }

		bool contains(const glm::vec3& pt) const;

		static AABB surroundingBox(const AABB& lhs, const AABB& rhs);

	private:
		glm::vec3 m_aa, m_bb;
	};
}

#endif