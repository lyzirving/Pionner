#ifndef __PIONNER_MESH_COMP_H__
#define __PIONNER_MESH_COMP_H__

#include "function/framework/comp/Comp.h"

namespace Pionner
{
	class MeshComp
	{
	public:
		MeshComp() : Comp() {};
		virtual ~MeshComp() = default;
	};
}

#endif