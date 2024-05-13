#ifndef __PIONNER_GFX_STRUCT_IMAGE_H__
#define __PIONNER_GFX_STRUCT_IMAGE_H__

#include "core/Buffer.h"
#include "gfx/rhi/RhiDef.h"

namespace pio
{
	enum class ImageInternalFormat : uint8_t
	{
		None = 0,
		FROM_FILE,

		RED, RED8, RED8UI, RED16UI, RED32UI, RED32F,
		RG, RG8, RG16F, RG32F,
		RGB, RGBUI, RGB16F, RGB32F,
		RGBA, RGBAUI, RGBA16F, RGBA32F,

		DEPTH24, DEPTH32F, DEPTH24STENCIL8,
	};

	enum class ImageUsage : uint8_t
	{
		None = 0, Texture, Attachment, Storage, HostRead
	};

	enum class PixelType : uint8_t
	{
		None = 0,
		UNSIGNED_BYTE, BYTE,
		UNSIGNED_SHORT, SHORT,
		UNSIGNED_INT, UNSIGNED_INT_24_8, INT,
		HALF_FLOAT, FLOAT
	};

	class Image
	{
	public:		
		static uint32_t GetChannelCount(ImageInternalFormat fmt);
		static PixelType GetPixelType(ImageInternalFormat imageFmt);
		static uint32_t GetPixelByteSize(PixelType type);
		static TextureInternalFmt GetInternalFmt(ImageInternalFormat imageFmt);
		static TextureFmt GetTextureFmt(ImageInternalFormat imageFmt);
		static ImageInternalFormat GetImageInternalFmt(uint32_t channelNum);

	public:
		Image() {}

	public:
		int32_t Width{ 0 }, Height{ 0 };
		int32_t Channel{ 0 };
		std::string	Name{}, Path{}, ParentDir{};
		AssetFmt AFmt{ AssetFmt::Num };

	public:
		void release();
		// Invalidate the data pointer, mainly used for transfering the control of data
		void invalidate() { m_data = nullptr; }
		bool loaded() const { return m_data != nullptr; };

	public:
		template<class T>
		T *Data() { return loaded() ? static_cast<T *>(m_data) : nullptr; }

	private:
		void *m_data{ nullptr };

		friend class ImageImporter;
	};
}

#endif