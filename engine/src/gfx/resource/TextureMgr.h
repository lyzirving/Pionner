#ifndef __PIONNER_GFX_RESOURCE_TEXTURE_MGR_H__
#define __PIONNER_GFX_RESOURCE_TEXTURE_MGR_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class RenderContext;
	class Texture;

	class TextureMgr
	{
	public:
		TextureMgr();
		~TextureMgr();

		void init(Ref<RenderContext>& context);
		void release();

		Ref<Texture> get(const std::string& name)
		{
			auto it = m_textures.find(name);
			if (it == m_textures.end())
			{
				return Ref<Texture>();
			}
			return it->second;
		}

	private:
		WeakRef<RenderContext> m_context;
		std::unordered_map<std::string, Ref<Texture>> m_textures;
	};
}

#endif