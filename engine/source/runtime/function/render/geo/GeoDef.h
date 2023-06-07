#ifndef __RENDER_GEO_DEF_H__
#define __RENDER_GEO_DEF_H__

#include <cstdint>

namespace Pionner
{
	enum GeometryType : uint8_t
	{
		GEO_TYPE_NONE,
		GEO_TYPE_INFINITE_GRID,
		GEO_TYPE_CYLINDER,
		GEO_TYPE_COORD_AXIS,
		GEO_TYPE_GROUP,
		GEO_TYPE_CNT
	};
}

#endif