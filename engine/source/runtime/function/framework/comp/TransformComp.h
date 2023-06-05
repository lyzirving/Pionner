#ifndef __PIONNER_TRANSFORM_COMP_H__
#define __PIONNER_TRANSFORM_COMP_H__

#include <atomic>

#include "core/math/Transform.h"
#include "function/framework/comp/Comp.h"

namespace Pionner
{
	class TransformComp : public Comp
	{
	public:
		TransformComp();
		virtual ~TransformComp();

		glm::mat4 getMat();

		void translate(float x, float y, float z);
		void rotate(float angle, float x, float y, float z);
		void scale(float x, float y, float z);

		const glm::vec3 getPosition();

	private:
		Transform m_transform;
	};
}

#endif