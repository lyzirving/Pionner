#include "GeometryComp.h"

namespace pio
{
	GeometryComp::GeometryComp() : Comp()
	{
	}

	GeometryComp::~GeometryComp()
	{
		m_geometry.reset();
	}
}