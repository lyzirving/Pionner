#include "Plane.h"

namespace pio
{
	Plane::Plane(const glm::vec3 &normal, const glm::vec3 &p0) : m_N(glm::normalize(normal))
	{
		m_D = -glm::dot(m_N, p0);
	}

	Plane::Plane(const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2)
	{
		glm::vec3 edge1 = glm::normalize(p1 - p0);
		glm::vec3 edge2 = glm::normalize(p2 - p0);
		m_N = glm::normalize(glm::cross(edge1, edge2));
		m_D = -glm::dot(m_N, p0);
	}
}