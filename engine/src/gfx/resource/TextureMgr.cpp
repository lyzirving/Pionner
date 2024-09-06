#include "TextureMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/Texture.h"

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
		whiteBuilder.name(GpuAttr::WHITE_TEXTURE)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGB_24)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer whiteBuff;
		whiteBuff.allocate(255, 1 * 1 * 3);		
		m_textures[GpuAttr::WHITE_TEXTURE] = Texture::Create(ctx, whiteBuilder.build(), whiteBuff);

		TextureSpecificBuilder blackBuilder;
		blackBuilder.name(GpuAttr::BLACK_TEXTURE)
			.type(TextureType::TwoDimen)
			.format(TextureFormat::RGB_24)
			.width(1).height(1)
			.texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
		Buffer blackBuff;
		blackBuff.allocate(0, 1 * 1 * 3);
		m_textures[GpuAttr::BLACK_TEXTURE] = Texture::Create(ctx, blackBuilder.build(), blackBuff);
	}

	void TextureMgr::release()
	{
		auto it = m_textures.begin();
		while(it != m_textures.end())
		{
			it->second.reset();
			it = m_textures.erase(it);
		}
	}
}