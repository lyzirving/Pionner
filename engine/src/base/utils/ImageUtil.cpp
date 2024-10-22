#include "ImageUtil.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageUtil"

namespace pio
{
	bool ImageUtil::GetPicInfo(const char* filePath, int32_t& w, int32_t& h, int32_t& comp)
	{
		return stbi_info(filePath, &w, &h, &comp) == 1;
	}

	bool ImageUtil::LoadImage(const std::string& path, void** data, int32_t& w, int32_t& h, int32_t& comp, int32_t reqComp)
	{		
		*data = stbi_load(path.c_str(), &w, &h, &comp, reqComp);
		return (*data) != nullptr;
	}

	void ImageUtil::FreeImage(void** data)
	{
		if (*data)
		{
			stbi_image_free(*data);
			*data = nullptr;
		}
	}
}