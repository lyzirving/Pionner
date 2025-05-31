#pragma once

#include "gfx/GfxDef.h"

namespace pio
{	
	class Texture;
	class RenderContext;
	struct TextureSpecific;

	class TextureMgr : public Object
	{
		RTTR_ENABLE(Object)
	public:
		TextureMgr(const Ref<RenderContext>& context);
		~TextureMgr();

		Ref<Texture> Get(const std::string& key);
		void Release();		

		Ref<Texture> Create(const TextureSpecific& spec);
		Ref<Texture> Create(const TextureSpecific& spec, Buffer& buffer);
		Ref<Texture> Create(const TextureSpecific& spec, uint8_t** buffer);
		Ref<Texture> Create(const TextureSpecific& spec, const std::string& path);

	private:
		WeakRef<RenderContext> m_Context;
		std::unordered_map<std::string, Ref<Texture>> m_Textures;
	};
}