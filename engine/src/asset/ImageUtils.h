#ifndef __PIONNER_ASSET_IMAGE_UTILS_H__
#define __PIONNER_ASSET_IMAGE_UTILS_H__

#include "Asset.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

namespace pio
{
	class ImageUtils
	{
	public:
		static bool GetPicInfo(const char *filePath, int32_t &width, int32_t &height, int32_t &component);
		static bool FillChannelData(uint8_t *src, uint8_t *dst, int32_t width, int32_t height, int32_t channelNum, int32_t reqChannel);
	};
}

#endif