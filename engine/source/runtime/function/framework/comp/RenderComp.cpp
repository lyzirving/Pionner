#include "function/framework/comp/RenderComp.h"
#include "RenderComp.h"

namespace Pionner
{
	Pionner::RenderComp::RenderComp() : Comp(), m_entity(nullptr)
	{
	}

	RenderComp::~RenderComp()
	{
		m_entity.reset();
	}
}