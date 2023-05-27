#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/math/Transform.h"
#include "core/math/MathLib.h"

namespace Pionner
{
	Rotation::Rotation()
		: m_mat(1.f)
		, m_axis(1.f, 0.f, 0.f), m_angle(0.f)
		, m_dirty(false)
	{
	}

	const glm::mat4 &Rotation::matrix()
	{
		if (m_dirty)
		{
			m_dirty = false;
			m_mat = glm::rotate(glm::radians(m_angle), m_axis);
		}
		return m_mat;
	}

	bool Rotation::rotate(float angle, const glm::vec3 &axis)
	{
		bool change{ false };
		if (!MathLib::equalF(angle, m_angle) || !MathLib::equalVec3(m_axis, axis))
		{
			m_angle = angle;
			m_axis = axis;
			change = m_dirty = true;
		}
		return change;
	}

	Transform::Transform()
		: m_position(0.f), m_scale(1.f), m_rotaion()
		, m_mat(1.f)
		, m_dirty(false)
	{
	}

	const glm::mat4 &Transform::matrix()
	{
		if (m_dirty)
		{
			m_dirty = false;
			glm::mat4 scaleMat = glm::scale(m_scale);
			glm::mat4 transMat = glm::translate(m_position);
			const glm::mat4 &rotateMat = m_rotaion.matrix();
			m_mat = transMat * rotateMat * scaleMat;
		}
		return m_mat;
	}

	bool Transform::position(float x, float y, float z)
	{
		bool change{ false };
		glm::vec3 pos{ x, y, z };
		if (!MathLib::equalVec3(pos, m_position))
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
		glm::vec3 scale{ x, y, z };
		if (!MathLib::equalVec3(scale, m_scale))
		{
			m_scale.x = x;
			m_scale.y = y;
			m_scale.z = z;
			change = m_dirty = true;
		}
		return change;
	}

	bool Transform::rotate(float angle, const glm::vec3 &axis)
	{
		bool change{ false };
		if (m_rotaion.rotate(angle, axis))
		{
			change = m_dirty = true;
		}
		return change;
	}
}