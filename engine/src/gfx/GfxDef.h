#ifndef __PIONNER_GFX_DEF_H__
#define __PIONNER_GFX_DEF_H__

#include "rhi/RhiDef.h"

namespace pio
{
	enum RenderingMode : uint8_t
	{
		RenderingMode_Opaque = 0,
		RenderingMode_Transparent,
		RenderingMode_Num
	};

	enum class RenderingStrategy : uint8_t
	{
		Forward = 0, Deffered
	};

	struct RenderConfig
	{
		RenderingStrategy Strategy{ RenderingStrategy::Deffered };
	};
}

#endif