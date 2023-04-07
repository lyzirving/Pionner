#include <glm/gtc/matrix_transform.hpp>
#include "core/math/Transform.h"

namespace Pionner
{
	Rotation::Rotation() : m_x(0.f, false), m_y(0.f, false), m_z(0.f, false)
		                 , m_mat(1.f)
		                 , m_dirty(false)
	{
	}

	const glm::mat4 &Rotation::matrix()
	{
		if (m_dirty)
		{
			m_dirty = false;
			m_mat = glm::mat4(1.f);
			m_mat = glm::rotate(m_mat, glm::radians(m_z.first), glm::vec3(0.f, 0.f, (m_z.second ? 1.f : -1.f)));
			m_mat = glm::rotate(m_mat, glm::radians(m_y.first), glm::vec3(0.f, (m_y.second ? 1.f : -1.f), 0.f));
			m_mat = glm::rotate(m_mat, glm::radians(m_x.first), glm::vec3((m_x.second ? 1.f : -1.f), 0.f, 0.f));
		}
		return m_mat;
	}

	bool Rotation::rotateByX(float angle, bool reverse)
	{
		bool change{ false };
		if (m_x.first != angle || m_x.second != reverse)
		{
			change = m_dirty = true;
			m_x.first = angle;
			m_x.second = reverse;
		}
		return change;
	}

	bool Rotation::rotateByY(float angle, bool reverse)
	{
		bool change{ false };
		if (m_y.first != angle || m_y.second != reverse)
		{
			change = m_dirty = true;
			m_y.first = angle;
			m_y.second = reverse;
		}
		return change;
	}

	bool Rotation::rotateByZ(float angle, bool reverse)
	{
		bool change{ false };
		if (m_z.first != angle || m_z.second != reverse)
		{
			change = m_dirty = true;
			m_z.first = angle;
			m_z.second = reverse;
		}
		return change;
	}

	Transform::Transform() : m_position(0.f), m_scale(1.f), m_rotaion()
		                   , m_mat(1.f), m_dirty(false)
	{
	}

	const glm::mat4 &Transform::matrix()
	{
		if (m_dirty)
		{
			m_dirty = false;
			m_mat = glm::mat4(1.f);
			// rotate first, then scale, finally translation
			m_mat = glm::translate(m_mat, m_position);
			m_mat = glm::scale(m_mat, m_scale);
			const glm::mat4 &rotateMat = m_rotaion.matrix();
			m_mat = m_mat * rotateMat;
		}
		return m_mat;
	}

	bool Transform::position(float x, float y, float z)
	{
		bool change{ false };
		if (m_position.x != x || m_position.y != y || m_position.z != z)
		{
			m_position.x = x;
			m_position.y = y;
			m_position.z = z;
			change = m_dirty = true;
		}
		return change;
	}

	bool Transform::scale(float x, float y, float z)
	{
		bool change{ false };
		if (m_scale.x != x || m_scale.y != y || m_scale.z != z)
		{
			m_scale.x = x;
			m_scale.y = y;
			m_scale.z = z;
			change = m_dirty = true;
		}
		return change;
	}

	bool Transform::rotateByX(float angle, bool reverse)
	{
		bool change{ false };
		if (m_rotaion.rotateByX(angle, reverse))
		{
			change = m_dirty = true;
		}
		return change;
	}

	bool Transform::rotateByY(float angle, bool reverse)
	{
		bool change{ false };
		if (m_rotaion.rotateByY(angle, reverse))
		{
			change = m_dirty = true;
		}
		return change;
	}

	bool Transform::rotateByZ(float angle, bool reverse)
	{
		bool change{ false };
		if (m_rotaion.rotateByZ(angle, reverse))
		{
			change = m_dirty = true;
		}
		return change;
	}
}