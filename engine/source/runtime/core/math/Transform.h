#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include <glm/glm.hpp>
#include <utility>

namespace Pionner
{
	class Rotation
	{
	public:
		Rotation();
		~Rotation() = default;

		inline const float angle() const { return m_angle; }
		inline const glm::vec3 axis() const { return m_axis; }

		const glm::mat4 &matrix();
		bool rotate(float angle, const glm::vec3 &axis);

	private:
		glm::mat4 m_mat;
		glm::vec3 m_axis;
		float     m_angle;
		bool      m_dirty;
	};

	class Transform
	{
	public:
		Transform();
		~Transform() = default;

		inline const glm::vec3 getPosition() const { return m_position; }
		inline const glm::vec3 getScale() const { return m_scale; }
		inline const float angle() const { return m_rotaion.angle(); }
		inline const glm::vec3 axis() const { return m_rotaion.axis(); }

		const glm::mat4 &matrix();

		bool position(float x, float y, float z);
		bool scale(float x, float y, float z);
		bool rotate(float angle, const glm::vec3 &axis);

	private:
		glm::vec3 m_position;
		glm::vec3 m_scale;
		Rotation  m_rotaion;
		glm::mat4 m_mat;
		bool      m_dirty;
	};
}

#endif