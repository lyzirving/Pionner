#ifndef __PIONNER_CORE_MATH_TRANSFORM_H__
#define __PIONNER_CORE_MATH_TRANSFORM_H__

#include "core/Base.h"
#include "core/math/EulerAngle.h"

namespace pio
{
	class Rotation
	{
	public:
		Rotation() {}
		~Rotation() = default;

		Rotation operator*(const Rotation &rhs);
		Rotation &operator=(const glm::quat &val);
		Rotation operator+(const glm::vec3 &euler);
		Rotation &operator+=(const glm::vec3 &euler);
			
		glm::quat quat() const { return m_quat; }
		glm::mat4 getMat();

	public:
		static Rotation FromQuat(const glm::quat &quat);
		static glm::quat ToQuat(float angle, const glm::vec3 &axis);
		static glm::quat ToQuat(const Rotation &rot);

	private:
		Rotation(float angle, const glm::vec3 &axis) : m_axis(axis), m_angle(angle), m_quat(ToQuat(angle, axis)), m_dirty(false) {}
		Rotation(const glm::quat &quat) : m_quat(quat) {}

		void flush();

	private:
		// pitch(rot around X), yaw(rot around Y), roll(rot around Z) in degree
		mutable glm::quat m_quat{ quaternion::IDENTITY };
		mutable bool m_dirty{ true };

		// Angle, axis are used for matrix calculation
		glm::vec3 m_axis{ 1.f, 0.f, 0.f };
		float m_angle{ 0.f };
	};

	class Transform
	{
	public:
		Transform() {};
		~Transform() = default;

		Transform operator*(const Transform &rhs);

		glm::mat4 getMat() { return glm::translate(glm::mat4(1.f),  Position) * 
								    Rotate.getMat() *
								    glm::scale(glm::mat4(1.f), Scale); }
	public:
		glm::vec3 Position{ 0.f };
		glm::vec3 Scale{ 1.f };
		Rotation Rotate{};	
		EulerAngle Euler;
	};
}

#endif