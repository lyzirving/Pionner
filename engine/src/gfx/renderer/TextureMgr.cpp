#include "TextureMgr.h"
#include "RenderContext.h"

#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "TextureMgr"

namespace pio
{
	TextureMgr::TextureMgr(const Ref<RenderContext>& context)
	{
		m_Context = context;
	}

	TextureMgr::~TextureMgr() = default;

	Ref<Texture> TextureMgr::Get(const std::string& key)
	{
		auto it = m_Textures.find(key);
		if(it == m_Textures.end())
		{
			return Ref<Texture>();
		}
		return it->second;
	}

	void TextureMgr::Release()
	{
		auto it = m_Textures.begin();
		while(it != m_Textures.end())
		{
			it->second.reset();
			it = m_Textures.erase(it);
		}
		m_Context.reset();
	}

	Ref<Texture> TextureMgr::Create(const TextureSpecific& spec)
	{
		auto texture = Get(spec.Name);
		auto ctx = m_Context.lock();
		if(!texture && ctx)
		{
			texture = Texture::Create(ctx, spec);
			m_Textures.insert({ texture->Name(), texture });
		}
		return texture;
	}

	Ref<Texture> TextureMgr::Create(const TextureSpecific& spec, Buffer& buffer)
	{
		auto texture = Get(spec.Name);
		auto ctx = m_Context.lock();
		if(!texture && ctx)
		{
			texture = Texture::Create(ctx, spec, buffer);
			m_Textures.insert({ texture->Name(), texture });
		}
		return texture;
	}

	Ref<Texture> TextureMgr::Create(const TextureSpecific& spec, uint8_t** buffer)
	{
		auto texture = Get(spec.Name);
		auto ctx = m_Context.lock();
		if(!texture && ctx)
		{
			texture = Texture::Create(ctx, spec, buffer);
			m_Textures.insert({ texture->Name(), texture });
		}
		return texture;
	}

	Ref<Texture> TextureMgr::Create(const TextureSpecific& spec, const std::string& path)
	{
		auto texture = Get(spec.Name);
		auto ctx = m_Context.lock();
		if (!texture && ctx)
		{
			texture = Texture::Create(ctx, spec, path);
			m_Textures.insert({ texture->Name(), texture });
		}
		return texture;
	}
}