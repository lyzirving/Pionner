#ifndef __PIONNER_BASE_EULER_ANGLE_H__
#define __PIONNER_BASE_EULER_ANGLE_H__

#include "defs/MathDef.h"

namespace pio
{
	class EulerAngle
	{
	public:
		EulerAngle() {}
		EulerAngle(const glm::vec3 &euler) : m_euler(Math::Reminder(euler, 360.f)) {}
		EulerAngle(float xRot, float yRot, float zRot) : m_euler(Math::Reminder(glm::vec3(xRot, yRot, zRot), 360.f)) {}
		~EulerAngle() = default;

		EulerAngle operator*(const EulerAngle &rhs);

		EulerAngle operator+(const EulerAngle &rhs);
		EulerAngle &operator+=(const EulerAngle &rhs);
		bool operator==(const EulerAngle &rhs);
		bool operator!=(const EulerAngle &rhs);

		EulerAngle operator+(const glm::vec3 &euler);		
		EulerAngle &operator+=(const glm::vec3 &euler);
		bool operator==(const glm::vec3 &euler);
		bool operator!=(const glm::vec3 &euler);
		
		EulerAngle &operator=(const glm::vec3 &euler);

		void invalidate(bool val = true) { m_dirty = val; }

		const glm::mat4& mat() const;
		const glm::quat& quat() const;
		const glm::vec3& angle() const { return m_euler; }
		bool bDirty() const { return m_dirty; }

		float pitch() const { return m_euler.x; }
		float yaw() const { return m_euler.y; }
		float roll() const { return m_euler.z; }

	private:
		void flush() const;

	private:
		//[TODO] Fix out why we need to correct euler angle??
		static glm::vec3 FixAngleLimit(const glm::vec3 &angle);

	private:
		// Pitch, Yaw, Roll in degree which represent rotation respectively around x,y,z axis
		glm::vec3 m_euler{ 0.f };
		mutable glm::quat m_quat{ quaternion::IDENTITY };
		mutable glm::mat4 m_rotMat{ 1.f };
		mutable bool m_dirty{ true };
	};
}

#endif