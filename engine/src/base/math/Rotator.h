#pragma once

#include "MathDef.h"

namespace pio
{
	class Rotator
	{
	public:
		Rotator() {}
		Rotator(const glm::vec3 &euler) : m_Euler(Math::Reminder(euler, 360.f)) {}
		Rotator(float xRot, float yRot, float zRot) : m_Euler(Math::Reminder(glm::vec3(xRot, yRot, zRot), 360.f)) {}
		~Rotator() = default;		

		bool operator==(const Rotator &rhs) const;
		bool operator!=(const Rotator &rhs) const;

		bool operator==(const glm::vec3& euler) const;
		bool operator!=(const glm::vec3& euler) const;

		Rotator& operator+=(const Rotator& rhs);
		Rotator& operator+=(const glm::vec3& euler);
		Rotator& operator=(const glm::vec3& euler);

		const glm::mat4& Mat() const;
		const glm::quat& Quat() const;
		const glm::vec3& Angle() const { return m_Euler; }

		float Pitch() const { return m_Euler.x; }
		float Yaw() const { return m_Euler.y; }
		float Roll() const { return m_Euler.z; }

	private:
		void Flush() const;

	private:
		//[TODO] Fix out why we need to correct euler angle??
		static glm::vec3 FixAngleLimit(const glm::vec3 &angle);

		friend Rotator operator*(const Rotator& second, const Rotator& first);
		friend Rotator operator+(const Rotator& lhs, const Rotator& rhs);

	private:
		// Pitch, Yaw, Roll in degree which represent rotation respectively around x,y,z axis
		glm::vec3 m_Euler{ 0.f };
		mutable glm::quat m_Quat{ quaternion::IDENTITY };
		mutable glm::mat4 m_RotMat{ 1.f };
		mutable bool m_bDirty{ true };
	};

	// perform rotation accumulation
	Rotator operator*(const Rotator& second, const Rotator& first);

	Rotator operator+(const Rotator& lhs, const Rotator& rhs);	
}