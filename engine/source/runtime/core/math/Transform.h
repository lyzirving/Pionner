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

		inline const std::pair<float, bool> rotateByX() const { return m_x; }
		inline const std::pair<float, bool> rotateByY() const { return m_y; }
		inline const std::pair<float, bool> rotateByZ() const { return m_z; }

		const glm::mat4 &matrix();
		bool rotateByX(float angle, bool reverse = false);
		bool rotateByY(float angle, bool reverse = false);
		bool rotateByZ(float angle, bool reverse = false);

	private:
		// first param is rotation angle, second param decides whether we reverse the axis.
		std::pair<float, bool> m_x;
		std::pair<float, bool> m_y;
		std::pair<float, bool> m_z;
		glm::mat4              m_mat;
		bool                   m_dirty;
	};

	class Transform
	{
	public:
		Transform();
		~Transform() = default;

		inline const glm::vec3& getPosition() const { return m_position; }
		inline const glm::vec3& getScale() const { return m_scale; }
		inline const std::pair<float, bool> getRotateByX() const { return m_rotaion.rotateByX(); }
		inline const std::pair<float, bool> getRotateByY() const { return m_rotaion.rotateByY(); }
		inline const std::pair<float, bool> getRotateByZ() const { return m_rotaion.rotateByZ(); }

		const glm::mat4 &matrix();
		bool position(float x, float y, float z);
		bool scale(float x, float y, float z);
		bool rotateByX(float angle, bool reverse = false);
		bool rotateByY(float angle, bool reverse = false);
		bool rotateByZ(float angle, bool reverse = false);

	private:
		glm::vec3 m_position;
		glm::vec3 m_scale;
		Rotation  m_rotaion;
		glm::mat4 m_mat;
		bool      m_dirty;
	};
}

#endif