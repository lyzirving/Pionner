#include "RenderComp.h"

namespace pio
{
	pio::RenderComp::RenderComp() : Comp(), m_entity(nullptr)
	{
	}

	RenderComp::~RenderComp()
	{
		m_entity.reset();
	}
}