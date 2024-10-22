#include "ImageUtil.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

namespace pio
{
	PIO_SINGLETON_IMPL(ImageUtil)

	ImageUtil::ImageUtil()
	{
	}

	ImageUtil::~ImageUtil() = default;

	void ImageUtil::Init()
	{
	}

	void ImageUtil::Shutdown()
	{
	}

	bool ImageUtil::GetPicInfo(const char* filePath, int32_t& width, int32_t& height, int32_t& comp)
	{
		return stbi_info(filePath, &width, &height, &comp) == 1;
	}
}