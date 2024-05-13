#include "Texture.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLTexture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Texture"

namespace pio
{
	Ref<Texture2D> Texture2D::Create(const std::string &path, const TextureSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLTexture2D>(path, spec);
			default:
				return Ref<Texture2D>();
		}
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification &spec, const Ref<Buffer> &buffer)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLTexture2D>(spec, buffer);
			default:
				return Ref<Texture2D>();
		}
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLTexture2D>(spec);
			default:
				return Ref<Texture2D>();
		}
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, uint32_t channelNum, int32_t val, const std::string &name)
	{
		uint32_t size = width * height * channelNum * sizeof(uint8_t);
		auto *data = (uint8_t *)std::malloc(size);
		std::memset(data, val, size);
		Ref<Buffer> buffer = CreateRef<Buffer>(data, size);

		TextureSpecification spec;
		spec.Name = name;
		spec.Format = Image::GetImageInternalFmt(channelNum);
		spec.Width = width; spec.Height = height;
		spec.MinFilter = TextureFilterMin::Linear;
		spec.MaxFilter = TextureFilterMag::Linear;
		spec.WrapS = TextureWrap::ClampEdge;
		spec.WrapT = TextureWrap::ClampEdge;
		spec.AType = AssetType::Texture2D;

		return Texture2D::Create(spec, buffer);		
	}

	Ref<CubeTexture> CubeTexture::Create(const TextureSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLCubeTexture>(spec);
			default:
				return Ref<CubeTexture>();
		}
	}

	Ref<CubeArrayTexture> CubeArrayTexture::Create(const TextureSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLCubeArrayTexture>(spec);
			default:
				return Ref<CubeArrayTexture>();
		}
	}

	Ref<BufferTexture> BufferTexture::Create(void *data, uint32_t byteSize, ImageInternalFormat format)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLBufferTexture>(data, byteSize, format);
			default:
				return Ref<BufferTexture>();
		}
	}

	Ref<Buffer> Texture::MakeBuffer(const TextureSpecification &spec, void *data)
	{		
		uint32_t byteSize = Image::GetPixelByteSize(Image::GetPixelType(spec.Format));
		uint32_t size = spec.Width * spec.Height * spec.Channel * byteSize;		
		Ref<Buffer> buffer = CreateRef<Buffer>(data, size);
		return buffer;
	}

	Ref<Buffer> Texture::MakeFlatBuffer(const TextureSpecification &spec, int32_t val)
	{
		uint32_t channlCount = Image::GetChannelCount(spec.Format);
		if (channlCount == 0)
		{
			LOGE("invalid formal[%u]", spec.Format);
			return Ref<Buffer>();
		}
		uint32_t size = spec.Height * spec.Width * channlCount;
		auto buffer = CreateRef<Buffer>();
		buffer->allocate(val, size);
		return buffer;
	}

	std::pair<uint32_t, uint32_t> Texture::GetMipSize(const TextureSpecification &spec, uint32_t mip)
	{
		uint32_t width = spec.Width;
		uint32_t height = spec.Height;
		while (mip != 0)
		{
			width /= 2;
			height /= 2;
			mip--;
		}
		return { width, height };
	}

	template<>
	bool Texture::is<Texture2D>() const { return getAssetType() == AssetType::Texture2D; }

	template<>
	bool Texture::is<CubeTexture>() const { return getAssetType() == AssetType::CubeTexture; }

	template<>
	bool Texture::is<CubeArrayTexture>() const { return getAssetType() == AssetType::CubeArrayTexture; }

	template<>
	bool Texture::is<BufferTexture>() const { return getAssetType() == AssetType::BufferTexture; }
}