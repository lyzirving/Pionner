#ifndef __PIONNER_GFX_RHI_TEXTURE_H__
#define __PIONNER_GFX_RHI_TEXTURE_H__

#include "asset/Asset.h"
#include "gfx/struct/Image.h"

namespace pio
{
	struct TextureSpecification
	{
		std::string Name;
		ImageInternalFormat Format = ImageInternalFormat::None;
		int32_t Width{ 1 };
		int32_t Height{ 1 };
		int32_t Channel{ 1 };
		int32_t Num{ 1 };
		TextureWrap WrapS{ TextureWrap::ClampEdge };
		TextureWrap WrapT{ TextureWrap::ClampEdge };
		TextureWrap WrapR{ TextureWrap::ClampEdge };
		TextureFilterMin MinFilter{ TextureFilterMin::Linear };
		TextureFilterMag MaxFilter{ TextureFilterMag::Linear };
		AssetType AType{ AssetType::Texture };
		glm::vec4 BorderColor{ 1.f };

		bool GenerateMips{ false };		
		bool SRGB{ true };// if false, linear rgb
		bool Storage{ false };
	};

	class Texture : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Texture)
	public:
		Texture() : Asset() {}
		virtual ~Texture() = default;

		virtual bool init() = 0;
		virtual void destroy() = 0;
		virtual bool isInit() const = 0;

		virtual void active(uint32_t sampler = 0) = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual ImageInternalFormat getFormat() const = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getSize() const = 0;

		virtual const std::string &getName() const = 0;
		virtual uint32_t getId() const = 0;
		virtual uint32_t getMipLevelCount() const = 0;
		virtual bool SRGB() const = 0;

	public:
		template<typename T>
		bool is() const { return false; };
		template<typename T>
		T *as() { if (is<T>()) { return static_cast<T *>(this); } else { return nullptr; } };

	public:
		static Ref<Buffer> MakeBuffer(const TextureSpecification &spec, void *data);
		static Ref<Buffer> MakeFlatBuffer(const TextureSpecification &spec, int32_t val);
		static std::pair<uint32_t, uint32_t> GetMipSize(const TextureSpecification &spec, uint32_t mip);
	};

	class Texture2D : public Texture
	{
		OVERRIDE_ASSET_TYPE(AssetType::Texture2D)
	public:
		static Ref<Texture2D> Create(const std::string &path, const TextureSpecification &spec);
		static Ref<Texture2D> Create(const TextureSpecification &spec, const Ref<Buffer> &buffer);
		static Ref<Texture2D> Create(const TextureSpecification &spec);

	public:
		Texture2D() : Texture() {}
		virtual ~Texture2D() = default;

		virtual Ref<Buffer> getBuffer() = 0;
	};

	class CubeTexture : public Texture2D
	{
		OVERRIDE_ASSET_TYPE(AssetType::CubeTexture)
	public:
		static Ref<CubeTexture> Create(const TextureSpecification &spec);

	public:
		CubeTexture() : Texture2D() {}
		virtual ~CubeTexture() = default;
	};

	class CubeArrayTexture : public Texture
	{
		OVERRIDE_ASSET_TYPE(AssetType::CubeArrayTexture)
	public:
		static Ref<CubeArrayTexture> Create(const TextureSpecification &spec);

	public:
		CubeArrayTexture() : Texture() {}
		virtual ~CubeArrayTexture() = default;
	};

	class BufferTexture : public Texture
	{
		OVERRIDE_ASSET_TYPE(AssetType::BufferTexture)
	public:
		static Ref<BufferTexture> Create(void *data, uint32_t byteSize, ImageInternalFormat format);

	public:
		BufferTexture() : Texture() {}
		virtual ~BufferTexture() = default;
	};

	template<>
	bool Texture::is<Texture2D>() const;

	template<>
	bool Texture::is<CubeTexture>() const;

	template<>
	bool Texture::is<CubeArrayTexture>() const;

	template<>
	bool Texture::is<BufferTexture>() const;
}

#endif