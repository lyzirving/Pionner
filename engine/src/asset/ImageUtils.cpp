#include "ImageUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageUtils"

namespace pio
{
	bool ImageUtils::GetPicInfo(const char *filePath, int32_t &width, int32_t &height, int32_t &component)
	{
		return stbi_info(filePath, &width, &height, &component) == 1;
	}

	bool ImageUtils::FillChannelData(uint8_t *src, uint8_t *dst, int32_t width, int32_t height, int32_t channelNum, int32_t reqChannel)
	{
		if (!src || !dst)
		{
			LOGE("invalid src or dst");
			return false;
		}
		if (reqChannel <= 0 || reqChannel > 4)
		{
			LOGE("invalid req channel[%d]", reqChannel);
			return false;
		}
		for (uint32_t i = 0; i < height; i++)
		{
			for (uint32_t j = 0; j < width; j++)
			{
				dst[(i * width + j) * channelNum + reqChannel - 1] = src[i * width + j];
			}
		}
		return true;
	}
}