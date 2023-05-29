#include <glm/gtx/transform.hpp>

#include "function/framework/comp/TransformComp.h"

namespace Pionner
{
	Pionner::TransformComp::TransformComp() : Comp()
		, m_transform()
	{
	}

	TransformComp::~TransformComp() = default;

	glm::mat4 TransformComp::getMat()
	{
		glm::mat4 ret = m_transform.matrix();
		return ret;
	}

	void TransformComp::translate(float x, float y, float z)
	{
		m_transform.position(x, y, z);
	}

	void TransformComp::rotate(float angle, float x, float y, float z)
	{
		m_transform.rotate(angle, glm::vec3(x, y, z));
	}

	void TransformComp::scale(float x, float y, float z)
	{
		m_transform.scale(x, y, z);
	}

	const glm::vec3 &TransformComp::getPosition()
	{
		glm::vec3 ret = m_transform.getPosition();
		return ret;
	}
}