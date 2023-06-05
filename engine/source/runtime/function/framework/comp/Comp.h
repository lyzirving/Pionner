#ifndef __PIONNER_COMP_H__
#define __PIONNER_COMP_H__

#include <glm/glm.hpp>

namespace Pionner
{
	struct CompArg
	{
		int       argInt;
		float     argFlt;
		glm::vec2 argVec2;
	};

	class Comp
	{
	public:
		Comp() {};
		virtual ~Comp() = default;
	};
}

#endif