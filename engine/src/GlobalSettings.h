#pragma once

#include "gfx/GfxDef.h"

namespace pio
{
	enum class ShadowResolutionSetting
	{
		Low, Medium, High
	};

	enum class ColorResolutionSetting
	{
		Low, Medium, High
	};

	enum class ColorAspectSetting
	{
		WH1_1, WH4_3, WH16_9
	};

	class GlobalSettings
	{
	public:
		static RenderConfig RenderConfig;
		static ShadowResolutionSetting ShadowResoSetting;
		static ColorResolutionSetting ColorResoSetting;
		static ColorAspectSetting AspectSetting;

		static uint32_t ShadowResolution(ShadowResolutionSetting setting);		
		static uint32_t ColorResolution(ColorResolutionSetting setting);
		static float AspectRatio(ColorAspectSetting setting);

		static float AspectRatio();
		static glm::uvec2 ColorResolution();
		static glm::uvec2 ShadowResolution(float aspect = AspectRatio());
		static glm::uvec2 PointLitShadowResolution();
	};
}