#pragma once

#include "Common.h"

struct aiMaterial;

namespace pio
{
	namespace MatAsset
	{
		constexpr const char* STANDARD = "standard";
		constexpr const char* TEXTURED = "textured";
	};

	enum MaterialFormat : uint8_t
	{
		MaterialFormat_None,
		MaterialFormat_Standard,
		MaterialFormat_Textured
	};
}