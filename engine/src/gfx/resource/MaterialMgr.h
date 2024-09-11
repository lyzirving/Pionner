#ifndef __PIONNER_GFX_RESOURCE_MATERIAL_MGR_H__
#define __PIONNER_GFX_RESOURCE_MATERIAL_MGR_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class RenderContext;
	class Material;

	class MaterialMgr
	{
	public:
		MaterialMgr();
		~MaterialMgr();

		void init(Ref<RenderContext>& context);
		void release();

		Ref<Material> get(const std::string& name)
		{
			auto it = m_materials.find(name);
			if(it == m_materials.end())
			{
				return Ref<Material>();
			}
			return it->second;
		}

	private:
		WeakRef<RenderContext> m_context;
		std::unordered_map<std::string, Ref<Material>> m_materials;
	};
}

#endif