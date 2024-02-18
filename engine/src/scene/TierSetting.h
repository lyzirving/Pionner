#ifndef __PIONNER_SCENE_TIER_SETTING_H__
#define __PIONNER_SCENE_TIER_SETTING_H__

#include "core/Base.h"

namespace pio
{
	namespace Tiering
	{
		enum class ShadowResolutionSetting
		{
			None = 0, Low = 1, Medium = 2, High = 3
		};

		enum class ColorResolutionSetting
		{
			None = 0, Low = 1, Medium = 2, High = 3
		};

		enum class ColorAspectSetting
		{
			WH1_1 = 0, WH4_3 = 1, WH16_9
		};

		inline uint32_t GetShadowResolution(ShadowResolutionSetting setting)
		{
			switch (setting)
			{
				case Tiering::ShadowResolutionSetting::Low:
					return 1024;
				case Tiering::ShadowResolutionSetting::Medium:
					return 2048;
				case Tiering::ShadowResolutionSetting::High:
					return 4096;
				case Tiering::ShadowResolutionSetting::None:
				default:
					return 0;
			}
		}

		inline uint32_t GetColorResolution(ColorResolutionSetting setting)
		{
			switch (setting)
			{
				case Tiering::ColorResolutionSetting::Low:
					return 1024;
				case Tiering::ColorResolutionSetting::Medium:
					return 2048;
				case Tiering::ColorResolutionSetting::High:
					return 4096;
				case Tiering::ColorResolutionSetting::None:
				default:
					return 0;
			}
		}

		inline float GetAspectRatio(ColorAspectSetting setting)
		{
			switch (setting)
			{
				case ColorAspectSetting::WH4_3:
					return 1.3333;
				case ColorAspectSetting::WH16_9:
					return 1.7777;
				case ColorAspectSetting::WH1_1:
				default:
					return 1.f;
			}
		}
	}
}

#endif