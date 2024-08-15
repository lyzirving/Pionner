#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include "core/Base.h"

namespace pio
{
	enum CRenderApiType : uint8_t
	{
		CRenderApiType_OpenGL = 0,
		CRenderApiType_Vulkan,
		CRenderApiType_Num
	};

	enum ProjectionType : uint8_t
	{
		ProjectionType_Perspective,
		ProjectionType_Orthographic,
		ProjectionType_Num
	};
}

#endif