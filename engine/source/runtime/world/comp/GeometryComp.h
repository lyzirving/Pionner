#ifndef __PIONNER_GEOMETRY_COMP_H__
#define __PIONNER_GEOMETRY_COMP_H__

#include <memory>

#include "Comp.h"
#include "render/geo/Geometry.h"

namespace pio
{
	class Geometry;

	class GeometryComp : public Comp
	{
	public:
		GeometryComp();
		virtual ~GeometryComp();

	public:
		std::shared_ptr<Geometry> m_geometry;
	};
}

#endif