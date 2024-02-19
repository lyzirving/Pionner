#include "ColorSpace.h"

namespace pio
{
	// average gamma for most case
	const float ColorSpace::Gamma = 2.2f;

	glm::vec4 ColorSpace::sRGBToLinearRGB(const glm::vec4 &sRGB)
	{
		return glm::vec4(std::pow(sRGB.r, Gamma),
						 std::pow(sRGB.g, Gamma),
						 std::pow(sRGB.b, Gamma),
						 std::pow(sRGB.a, Gamma));
	}
}