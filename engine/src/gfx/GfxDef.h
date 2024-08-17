#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include "Base.h"

namespace pio
{
	enum BackendFlags : uint8_t
	{
		Backend_OpenGL = 0,
		Backend_Vulkan,
		Backend_Num
	};

	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
	};
}

#endif