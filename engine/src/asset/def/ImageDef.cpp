#include "ImageDef.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageDef"

namespace pio
{
	uint32_t GetTextureChannelNum(TextureFormat fmt)
	{
		switch(fmt)
		{
			case TextureFormat::RGBA_32:
			case TextureFormat::RGBA_FLOAT:
				return 4;
			case TextureFormat::RGB_24:
			case TextureFormat::RGBA_HALF:
			case TextureFormat::RGB_16:
			case TextureFormat::RGB_16F:
				return 3;
			case TextureFormat::RG_FLOAT:
			case TextureFormat::DEPTH_24_STENCIL_8:
				return 2;
			case TextureFormat::R_16:
			case TextureFormat::R_8:
			case TextureFormat::R_FLOAT:
			case TextureFormat::Alpha_8:
			case TextureFormat::DEPTH_16:
			case TextureFormat::DEPTH_24:
			case TextureFormat::DEPTH_32:
			case TextureFormat::DEPTH_32F:
				return 1;
			default:
				LOGE("err! texture format[%u] has not been implemented", fmt);
				std::abort();
				return 0;
		}
	}

	uint32_t GetTextureByteSize(TextureFormat fmt)
	{
		switch(fmt)
		{
			case TextureFormat::RGBA_FLOAT:
			case TextureFormat::RG_FLOAT:
			case TextureFormat::R_FLOAT:
			case TextureFormat::DEPTH_32:
			case TextureFormat::DEPTH_32F:
			case TextureFormat::DEPTH_24_STENCIL_8:
				return 4;
			case TextureFormat::DEPTH_24:
				return 3;
			case TextureFormat::RGBA_HALF:
			case TextureFormat::RGB_16:
			case TextureFormat::RGB_16F:
			case TextureFormat::R_16:
			case TextureFormat::DEPTH_16:
				return 2;
			case TextureFormat::RGBA_32:
			case TextureFormat::RGB_24:
			case TextureFormat::R_8:
			case TextureFormat::Alpha_8:
				return 1;
			default:
				LOGE("err! texture format[%u] has not been implemented", fmt);
				std::abort();
				return 0;
		}
	}

	ImageFormat GetFormatByChannal(int32_t comp)
	{
		switch (comp)
		{
			case 1:
				return ImageFormat_BITMAP;
			case 3:
				return ImageFormat_PNG;
			case 4:
				return ImageFormat_JPEG;
			default:
				LOGE("err! unsupported comp[%d]", comp);
				std::abort();
				return ImageFormat_None;
		}
	}
}