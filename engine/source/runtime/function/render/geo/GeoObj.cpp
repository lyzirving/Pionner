#include "function/render/geo/GeoObj.h"

namespace Pionner
{
	GeoObj::GeoObj(const char *name) : m_name(name), m_meshComp(nullptr)
	{
	}

	GeoObj::~GeoObj()
	{
		m_meshComp.reset();
	}
}