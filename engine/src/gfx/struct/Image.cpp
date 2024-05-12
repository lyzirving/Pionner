#include "Image.h"

#include "asset/ImageUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Image"

namespace pio
{
	uint32_t Image::GetChannelCount(ImageInternalFormat fmt)
	{
		switch (fmt)
		{
			case ImageInternalFormat::RED:
			case ImageInternalFormat::RED8:
			case ImageInternalFormat::RED8UI:
			case ImageInternalFormat::RED16UI:
			case ImageInternalFormat::RED32UI:
			case ImageInternalFormat::RED32F:
			case ImageInternalFormat::DEPTH24:
			case ImageInternalFormat::DEPTH32F:
				return 1;
			case ImageInternalFormat::RG:
			case ImageInternalFormat::RG8:
			case ImageInternalFormat::RG16F:
			case ImageInternalFormat::RG32F:
			case ImageInternalFormat::DEPTH24STENCIL8:
				return 2;
			case ImageInternalFormat::RGB:
			case ImageInternalFormat::RGBUI:
			case ImageInternalFormat::RGB16F:
			case ImageInternalFormat::RGB32F:
				return 3;
			case ImageInternalFormat::RGBA:
			case ImageInternalFormat::RGBAUI:
			case ImageInternalFormat::RGBA16F:
			case ImageInternalFormat::RGBA32F:
				return 4;
			case ImageInternalFormat::None:
			default:
				return 0;
		}
	}

	PixelType Image::GetPixelType(ImageInternalFormat imageFmt)
	{
		switch (imageFmt)
		{
			case ImageInternalFormat::RED:
			case ImageInternalFormat::RG:
			case ImageInternalFormat::RGB:
			case ImageInternalFormat::RGBA:
			case ImageInternalFormat::RED8UI:
				return PixelType::UNSIGNED_BYTE;
			case ImageInternalFormat::RED8:
			case ImageInternalFormat::RG8:
				return PixelType::BYTE;
			case ImageInternalFormat::RED16UI:
				return PixelType::UNSIGNED_SHORT;
			case ImageInternalFormat::RED32UI:
			case ImageInternalFormat::RGBUI:
			case ImageInternalFormat::RGBAUI:
				return PixelType::UNSIGNED_INT;
			case ImageInternalFormat::RG16F:
			case ImageInternalFormat::RGBA16F:
				return PixelType::HALF_FLOAT;
			case ImageInternalFormat::DEPTH24:
			case ImageInternalFormat::RED32F:
			case ImageInternalFormat::RG32F:
			case ImageInternalFormat::RGB16F:
			case ImageInternalFormat::RGB32F:
			case ImageInternalFormat::RGBA32F:
			case ImageInternalFormat::DEPTH32F:
				return PixelType::FLOAT;
			case ImageInternalFormat::DEPTH24STENCIL8:
				return PixelType::UNSIGNED_INT_24_8;
			default:
				LOGE("invalid image format[%u]", imageFmt);
				return PixelType::None;
		}
	}

	uint32_t Image::GetPixelByteSize(PixelType type)
	{
		switch (type)
		{
			case PixelType::UNSIGNED_BYTE:
			case PixelType::BYTE:
				return 1;
			case PixelType::UNSIGNED_SHORT:
			case PixelType::SHORT:
			case PixelType::HALF_FLOAT:
				return 2;
			case PixelType::UNSIGNED_INT:
			case PixelType::UNSIGNED_INT_24_8:
			case PixelType::INT:
			case pio::PixelType::FLOAT:
				return 4;
			default:
				LOGE("invalid pixel type[%u]", type);
				return 0;
		}
	}

	TextureInternalFmt Image::GetInternalFmt(ImageInternalFormat imageFmt)
	{
		switch (imageFmt)
		{
			case ImageInternalFormat::RED:
				return TextureInternalFmt::RED;
			case ImageInternalFormat::RED8:
				return TextureInternalFmt::R8;
			case ImageInternalFormat::RED8UI:
				return TextureInternalFmt::R8U;
			case ImageInternalFormat::RED16UI:
				return TextureInternalFmt::R16U;
			case ImageInternalFormat::RED32UI:
				return TextureInternalFmt::R32U;
			case ImageInternalFormat::RED32F:
				return TextureInternalFmt::R32F;
			case ImageInternalFormat::RG:
				return TextureInternalFmt::RG;
			case ImageInternalFormat::RG8:
				return TextureInternalFmt::RG8;
			case ImageInternalFormat::RG16F:
				return TextureInternalFmt::RG16F;
			case ImageInternalFormat::RG32F:
				return TextureInternalFmt::RG32F;
			case ImageInternalFormat::RGB:
				return TextureInternalFmt::RGB;
			case ImageInternalFormat::RGBUI:
				return TextureInternalFmt::RGBUI;
			case ImageInternalFormat::RGB16F:
				return TextureInternalFmt::RGB16F;
			case ImageInternalFormat::RGB32F:
				return TextureInternalFmt::RGB32F;
			case ImageInternalFormat::RGBA:
				return TextureInternalFmt::RGBA;
			case ImageInternalFormat::RGBA16F:
				return TextureInternalFmt::RGBA16F;
			case ImageInternalFormat::RGBA32F:
				return TextureInternalFmt::RGBA32F;
			case ImageInternalFormat::RGBAUI:
				return TextureInternalFmt::RGBAUI;
			case ImageInternalFormat::DEPTH24:
				return TextureInternalFmt::Depth24;
			case ImageInternalFormat::DEPTH32F:
				return TextureInternalFmt::Depth32F;
			case ImageInternalFormat::DEPTH24STENCIL8:
				return TextureInternalFmt::DepthStencil;
			default:
				LOGE("invalid imgae format[%u]", imageFmt);
				return TextureInternalFmt::None;
		}
	}

	TextureFmt Image::GetTextureFmt(ImageInternalFormat imageFmt)
	{
		switch (imageFmt)
		{
			case ImageInternalFormat::RED:
			case ImageInternalFormat::RED8:
			case ImageInternalFormat::RED8UI:
			case ImageInternalFormat::RED16UI:
			case ImageInternalFormat::RED32UI:
			case ImageInternalFormat::RED32F:
				return TextureFmt::RED;
			case ImageInternalFormat::RG:
			case ImageInternalFormat::RG8:
			case ImageInternalFormat::RG16F:
			case ImageInternalFormat::RG32F:
				return TextureFmt::RG;
			case ImageInternalFormat::RGB:
			case ImageInternalFormat::RGB16F:
			case ImageInternalFormat::RGBUI:
			case ImageInternalFormat::RGB32F:
				return TextureFmt::RGB;
			case ImageInternalFormat::RGBA:
			case ImageInternalFormat::RGBAUI:
			case ImageInternalFormat::RGBA16F:
			case ImageInternalFormat::RGBA32F:
				return TextureFmt::RGBA;
			case ImageInternalFormat::DEPTH24:
			case ImageInternalFormat::DEPTH32F:
				return TextureFmt::Depth;
			case ImageInternalFormat::DEPTH24STENCIL8:
				return TextureFmt::DepthStencil;
			default:
				LOGE("invalid image fmt[%u]", imageFmt);
				return TextureFmt::None;
		}
	}

	void Image::release()
	{
		if (m_data) { stbi_image_free(m_data); m_data = nullptr; }
	}
}