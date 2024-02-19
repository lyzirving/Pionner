#ifndef __PIONNER_CORE_UTILS_COLOR_SPACE_H__
#define __PIONNER_CORE_UTILS_COLOR_SPACE_H__

#include "core/math/MathLib.h"

namespace pio
{
	class ColorSpace
	{
	public:
		static const float Gamma;
		static glm::vec4 sRGBToLinearRGB(const glm::vec4 &sRGB);
	private:
		ColorSpace() {}
		~ColorSpace() = default;
	};
}

#endif