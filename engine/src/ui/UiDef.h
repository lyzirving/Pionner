#ifndef __PIONNER_UI_UIDEF_H__
#define __PIONNER_UI_UIDEF_H__

#include "asset/Asset.h"

namespace pio
{

	#define CLICK_INTERVAL (300)

	constexpr static const char *UI_AXIS_X = "UI_AxisX";
	constexpr static const char *UI_AXIS_Y = "UI_AxisY";
	constexpr static const char *UI_AXIS_Z = "UI_AxisZ";

	constexpr static const char *UI_TORUS_X = "UI_TorusX";
	constexpr static const char *UI_TORUS_Y = "UI_TorusY";
	constexpr static const char *UI_TORUS_Z = "UI_TorusZ";

	constexpr static const char *UI_DIST_LIGHT = "UI_DistantLight";
	constexpr static const char *UI_POINT_LIGHT = "UI_PointLight";

	enum class CoordinateShape : uint8_t
	{
		Cylinder, Arrow, Num
	};
}

#endif