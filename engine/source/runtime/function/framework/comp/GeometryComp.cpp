#include "GeometryComp.h"

namespace Pionner
{
	GeometryComp::GeometryComp() : Comp()
	{
	}

	GeometryComp::~GeometryComp()
	{
		m_geometry.reset();
	}
}