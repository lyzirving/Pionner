#include "EulerAngle.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "EulerAngle"

namespace pio
{
	#define ANGLE_LIMIT (302.f)

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

	EulerAngle &EulerAngle::operator+=(const EulerAngle &rhs)
	{
		*this += rhs.angle();
		return *this;
	}

	EulerAngle &EulerAngle::operator=(const glm::vec3 &euler)
	{
		if (euler != m_euler)
		{
			m_euler = Math::Reminder(euler, 360.f);
			m_dirty = true;
		}
		return *this;
	}

	glm::mat4 EulerAngle::mat() const
	{
		flush();
		return m_rotMat;
	}

	glm::quat EulerAngle::quat() const
	{
		flush();
		return m_quat;
	}

	void EulerAngle::flush() const
	{
		if (!m_dirty)
			return;

		glm::vec3 angle = FixAngleLimit(m_euler);
		m_quat = glm::quat(glm::radians(angle));
		m_rotMat = glm::rotate(glm::mat4(1.f), glm::angle(m_quat), glm::axis(m_quat));
		m_dirty = false;
	}

	glm::vec3 EulerAngle::FixAngleLimit(const glm::vec3 &angle)
	{
		// val is in [-360.f, 360.f]
		auto doCorrect = [](float val) 
		{			
			return val >= ANGLE_LIMIT ? (val - 360.f) : (val <= -ANGLE_LIMIT) ? (360.f + val) : val;
		};
		glm::vec3 result = angle;
		result.x = doCorrect(result.x);
		result.y = doCorrect(result.y);
		result.z = doCorrect(result.z);
		return result;
	}
}