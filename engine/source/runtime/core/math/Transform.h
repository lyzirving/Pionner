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

		const glm::mat4 &matrix();
		bool rotateX(float angle, bool reverse = false);
		bool rotateY(float angle, bool reverse = false);
		bool rotateZ(float angle, bool reverse = false);

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

		const glm::mat4 &matrix();
		void position(float x, float y, float z);
		void scale(float x, float y, float z);
		void rotateX(float angle, bool reverse = false);
		void rotateY(float angle, bool reverse = false);
		void rotateZ(float angle, bool reverse = false);

	private:
		glm::vec3 m_position;
		glm::vec3 m_scale;
		Rotation  m_rotaion;
		glm::mat4 m_mat;
		bool      m_dirty;
	};
}

#endif