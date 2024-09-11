#include "MaterialMgr.h"
#include "Material.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialMgr"

namespace pio
{
	MaterialMgr::MaterialMgr()
	{
	}

	MaterialMgr::~MaterialMgr() = default;

	void MaterialMgr::init(Ref<RenderContext>& context)
	{
		m_context = context;
		m_materials.insert({ GpuAttr::STANDARD_MATERIAL, Material::MakeStandardMaterial(GpuAttr::STANDARD_MATERIAL) });
	}

	void MaterialMgr::release()
	{
		auto it = m_materials.begin();
		while(it != m_materials.end())
		{
			it->second.reset();
			it = m_materials.erase(it);
		}
		m_context.reset();
	}
}