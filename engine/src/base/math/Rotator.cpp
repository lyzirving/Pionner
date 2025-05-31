#include "Rotator.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Rotator"

namespace pio
{
	#define ANGLE_LIMIT (302.f)

	bool Rotator::operator==(const Rotator &rhs) const
	{
		return this != &rhs && this->m_Euler == rhs.m_Euler;
	}

	bool Rotator::operator!=(const Rotator &rhs) const
	{
		return !(*this == rhs);
	}

	bool Rotator::operator==(const glm::vec3& euler) const
	{
		return this->m_Euler == euler;
	}

	bool Rotator::operator!=(const glm::vec3& euler) const
	{
		return !(*this == euler);
	}

	Rotator& Rotator::operator+=(const Rotator& rhs)
	{
		*this += rhs.Angle();
		return *this;
	}

	Rotator& Rotator::operator+=(const glm::vec3& euler)
	{
		m_Euler = Math::Reminder(m_Euler + euler, 360.f);
		m_bDirty = true;
		return *this;
	}

	Rotator &Rotator::operator=(const glm::vec3 &euler)
	{
		if (euler != m_Euler)
		{
			m_Euler = Math::Reminder(euler, 360.f);
			m_bDirty = true;
		}
		return *this;
	}

	const glm::mat4& Rotator::Mat() const
	{
		Flush();
		return m_RotMat;
	}

	const glm::quat& Rotator::Quat() const
	{
		Flush();
		return m_Quat;
	}

	void Rotator::Flush() const
	{
		if (!m_bDirty)
			return;

		glm::vec3 angle = FixAngleLimit(m_Euler);
		m_Quat = glm::quat(glm::radians(angle));
		m_RotMat = glm::rotate(glm::mat4(1.f), glm::angle(m_Quat), glm::axis(m_Quat));
		m_bDirty = false;
	}

	glm::vec3 Rotator::FixAngleLimit(const glm::vec3 &angle)
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

	Rotator operator*(const Rotator& second, const Rotator& first)
	{
		Rotator rotator(glm::degrees(glm::eulerAngles(second.Quat() * first.Quat())));
		return rotator;
	}

	Rotator operator+(const Rotator& lhs, const Rotator& rhs)
	{
		Rotator rotator(lhs.m_Euler + rhs.m_Euler);
		return rotator;
	}

}