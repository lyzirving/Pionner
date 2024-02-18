#ifndef __PIONNER_SCENE_SCENE_DEF_H__
#define __PIONNER_SCENE_SCENE_DEF_H__

#include "core/Base.h"

namespace pio
{
	#define POINT_LIGHT_LIMIT 10

	enum class LightType : uint8_t
	{
		None = 0, Directional, Point, Spot, Num
	};

	enum ShadowMode : uint8_t
	{
		ShadowMode_Hard = 0, ShadowMode_Soft, ShadowMode_Soft_2X, ShadowMode_Soft_4X, ShadowMode_Num
	};

	constexpr static const char *ShadowModeNames[ShadowMode_Num] = { "Hard", "Soft", "Soft_2X", "Soft_4X" };
}

#endif