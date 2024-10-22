#include "TextureMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Texture.h"

#include "asset/AssetMgr.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TextureMgr"

namespace pio
{
	TextureMgr::TextureMgr()
	{
	}

	TextureMgr::~TextureMgr() = default;

	void TextureMgr::init(Ref<RenderContext>& context)
	{
		m_context = context;
		auto ctx = m_context.lock();
		PIO_CHECK_RETURN(ctx, "RenderContext is invalid");

		TextureSpecificBuilder whiteBuilder;
		whiteBuilder.name(GpuAttr::Tex::WHITE)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_32)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer whiteBuff;
		whiteBuff.allocate(255, 1 * 1 * 4);	
		m_textures[GpuAttr::Tex::WHITE] = Texture::Create(ctx, whiteBuilder.build(), whiteBuff);

		TextureSpecificBuilder blackBuilder;
		blackBuilder.name(GpuAttr::Tex::BLACK)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGBA_32)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer blackBuff;
		blackBuff.allocate(0, 1 * 1 * 4);
		m_textures[GpuAttr::Tex::BLACK] = Texture::Create(ctx, blackBuilder.build(), blackBuff);

		TextureSpecificBuilder distLitBuilder;
		distLitBuilder.name(GpuAttr::Tex::DIST_LIGHT)
			.type(TextureType::TwoDimen)
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear)
			.flipV(true);
		m_textures[GpuAttr::Tex::DIST_LIGHT] = Texture::Create(ctx, distLitBuilder.build(), AssetMgr::SpritePath(GpuAttr::Tex::DIST_LIGHT, ImageType::PNG));

		Buffer metallicRoughnessBuff;
		metallicRoughnessBuff.allocate(255, 1 * 1 * 3);
		auto* data = metallicRoughnessBuff.as<uint8_t>();
		ImageUtil::FillSingleChannel(data, 3, 0, 1, 1, (uint8_t)0);
		TextureSpecificBuilder metallicRough;
		metallicRough.name(GpuAttr::Tex::DEFAULT_METALLIC_ROUGHNESS)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGB_24)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		m_textures[GpuAttr::Tex::DEFAULT_METALLIC_ROUGHNESS] = Texture::Create(ctx, metallicRough.build(), metallicRoughnessBuff);
	}

	void TextureMgr::release()
	{
		auto it = m_textures.begin();
		while(it != m_textures.end())
		{
			it->second.reset();
			it = m_textures.erase(it);
		}
		m_context.reset();
	}

	Ref<Texture> TextureMgr::create(const TextureSpecific& spec)
	{
		auto texture = get(spec.Name);
		auto ctx = m_context.lock();
		if(!texture && ctx)
		{
			texture = Texture::Create(ctx, spec);
			m_textures.insert({ texture->name(), texture });
		}
		return texture;
	}

	Ref<Texture> TextureMgr::create(const TextureSpecific& spec, Buffer& buffer)
	{
		auto texture = get(spec.Name);
		auto ctx = m_context.lock();
		if(!texture && ctx)
		{
			texture = Texture::Create(ctx, spec, buffer);
			m_textures.insert({ texture->name(), texture });
		}
		return texture;
	}

	Ref<Texture> TextureMgr::create(const TextureSpecific& spec, const std::string& path)
	{
		auto texture = get(spec.Name);
		auto ctx = m_context.lock();
		if (!texture && ctx)
		{
			texture = Texture::Create(ctx, spec, path);
			m_textures.insert({ texture->name(), texture });
		}
		return texture;
	}
}