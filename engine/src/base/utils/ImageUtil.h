#ifndef __PIONNER_BASE_UTILS_IMAGE_UTIL_H__
#define __PIONNER_BASE_UTILS_IMAGE_UTIL_H__

#include "base/defs/FunDef.h"

namespace pio
{
	class ImageUtil
	{
		PIO_SINGLETON_DECLARE(ImageUtil)
	public:
		static bool GetPicInfo(const char* filePath, int32_t& width, int32_t& height, int32_t& comp);
	};
}

#endif