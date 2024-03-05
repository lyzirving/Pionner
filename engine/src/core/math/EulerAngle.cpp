#include "EulerAngle.h"

namespace pio
{
	EulerAngle EulerAngle::operator*(const EulerAngle &rhs)
	{
		EulerAngle e(this->m_euler + rhs.m_euler);
		return e;
	}

	EulerAngle EulerAngle::operator+(const glm::vec3 &euler)
	{
		EulerAngle e(this->m_euler + euler);
		return e;
	}

	EulerAngle &EulerAngle::operator+=(const glm::vec3 &euler)
	{
		m_euler = Math::Reminder(m_euler + euler, 360.f);
		m_dirty = true;
		return *this;
	}

	EulerAngle &EulerAngle::operator=(const glm::vec3 &euler)
	{
		m_euler = Math::Reminder(euler, 360.f);
		m_dirty = true;
		return *this;
	}

	glm::mat4 EulerAngle::getMat() const
	{
		flush();
		return m_rotMat;
	}

	void EulerAngle::flush() const
	{
		if (!m_dirty)
			return;

		m_quat = glm::quat(glm::radians(m_euler));
		m_rotMat = glm::rotate(glm::mat4(1.f), glm::angle(m_quat), glm::axis(m_quat));
		m_dirty = false;
	}
}