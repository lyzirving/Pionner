#include "GlobalSettings.h"

namespace pio
{
	RenderConfig GlobalSettings::RenderConfig{};
	ShadowResolutionSetting GlobalSettings::ShadowResoSetting{ ShadowResolutionSetting::Low };
	ColorResolutionSetting GlobalSettings::ColorResoSetting{ ColorResolutionSetting::Low };
	ColorAspectSetting GlobalSettings::AspectSetting{ ColorAspectSetting::WH4_3 };

	uint32_t GlobalSettings::ShadowResolution(ShadowResolutionSetting setting)
	{
		switch (setting)
		{
			case ShadowResolutionSetting::Low:
				return 1024;
			case ShadowResolutionSetting::Medium:
				return 2048;
			case ShadowResolutionSetting::High:
				return 4096;
			default:
				return 0;
		}
	}

	uint32_t GlobalSettings::ColorResolution(ColorResolutionSetting setting)
	{
		switch (setting)
		{
			case ColorResolutionSetting::Low:
				return 1024;
			case ColorResolutionSetting::Medium:
				return 2048;
			case ColorResolutionSetting::High:
				return 4096;
			default:
				return 0;
		}
	}

	float GlobalSettings::AspectRatio(ColorAspectSetting setting)
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