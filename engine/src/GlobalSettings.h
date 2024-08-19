#ifndef __PIONNER_GLOBAL_SETTINGS_H__
#define __PIONNER_GLOBAL_SETTINGS_H__

#include "Base.h"

namespace pio
{
	enum RenderingMode : uint8_t
	{
		RenderingMode_Forward = 0,
		RenderingMode_Deffered
	};

	struct RenderConfig
	{
		RenderingMode Mode{ RenderingMode_Deffered };
	};

	class GlobalSettings
	{
	public:
		static RenderConfig k_RenderConfig;
	};
}

#endif