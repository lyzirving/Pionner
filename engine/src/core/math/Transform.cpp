#include "Transform.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Transform"

namespace pio
{
	void Rotation::flush()
	{
		if (!m_dirty)
			return;

		Rotation r = FromQuat(m_quat);
		m_axis = r.m_axis;
		m_angle = r.m_angle;
		m_dirty = false;
	}

	Rotation Rotation::operator*(const Rotation &rhs)
	{				
		glm::quat self = this->m_dirty ? this->m_quat : ToQuat(this->m_angle, this->m_axis);
		glm::quat __rhs = rhs.m_dirty ? rhs.m_quat : ToQuat(rhs.m_angle, rhs.m_axis);
		Rotation r = self * __rhs;
		return r;
	}

	Rotation &Rotation::operator=(const glm::quat &val)
	{
		if (m_quat != val)
		{
			m_quat = val;
			m_dirty = true;
		}
		return *this;
	}

	Rotation Rotation::operator+(const glm::vec3 &euler)
	{
		Rotation r(this->m_quat);
		// pitch(rot around X), yaw(rot around Y), roll(rot around Z) in radian
		glm::quat __quat(glm::vec3(glm::radians(euler)));
		
		r.m_quat = __quat * r.m_quat;
		r.m_dirty = true;
		return r;
	}

	Rotation &Rotation::operator+=(const glm::vec3 &euler)
	{
		// pitch(rot around X), yaw(rot around Y), roll(rot around Z) in radian
		glm::quat __quat(glm::vec3(glm::radians(euler)));
		this->m_quat = __quat * this->m_quat;
		this->m_dirty = true;
		return *this;
	}

	glm::mat4 Rotation::getMat()
	{
		flush();
		return glm::rotate(glm::mat4(1.f), glm::radians(m_angle), m_axis);
	}

	Rotation Rotation::FromQuat(const glm::quat &quat)
	{
		return Rotation(glm::degrees(glm::angle(quat)), glm::axis(quat));
	}

	glm::quat Rotation::ToQuat(float angle, const glm::vec3 &axis)
	{
		float halfAngle = glm::radians(angle * 0.5f);
		glm::vec3 dir = glm::normalize(axis);
		return glm::quat(std::cos(halfAngle),
						 std::sin(halfAngle) * dir.x,
						 std::sin(halfAngle) * dir.y,
						 std::sin(halfAngle) * dir.z);
	}

	glm::quat Rotation::ToQuat(const Rotation &rot)
	{
		return ToQuat(rot.m_angle, rot.m_axis);
	}

	Transform Transform::operator*(const Transform &rhs)
	{
		Transform ret;
		ret.Position = this->Position + rhs.Position;
		ret.Rotate = this->Rotate * rhs.Rotate;
		ret.Scale = this->Scale * rhs.Scale;
		return ret;
	}
}