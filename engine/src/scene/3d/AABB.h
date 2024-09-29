#ifndef __PIONNER_SCENE_3D_AABB_H__
#define __PIONNER_SCENE_3D_AABB_H__

#include "Common.h"

namespace pio
{
	class AABB
	{
	public:
		AABB() {}
		AABB(const glm::vec3& min, const glm::vec3& max) : m_min(min), m_max(max) {}
		~AABB() = default;

		const glm::vec3& min() const { return m_min; }
		const glm::vec3& max() const { return m_max; }

	private:
		glm::vec3 m_min{ 0.f }, m_max{ 1.f };
	};
}

#endif