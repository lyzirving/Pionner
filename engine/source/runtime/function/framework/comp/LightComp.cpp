#include "LightComp.h"

namespace Pionner
{
	LightComp::LightComp() : Comp()
		, m_light(nullptr)
		, m_entity(nullptr)
	{
	}

	LightComp::~LightComp()
	{
		m_light.reset();
		m_entity.reset();
	}
}