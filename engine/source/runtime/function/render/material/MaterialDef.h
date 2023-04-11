#ifndef __MATERIAL_DEF_H__
#define __MATERIAL_DEF_H__

#include <cstdint>

namespace Pionner
{
	enum MaterialType : uint8_t
	{
		MAT_NONE,
		MAT_DIFFUSE,
		MAT_SPECULAR,
		MAT_AMBIENT,
		MAT_COUNT
	};
}

#endif