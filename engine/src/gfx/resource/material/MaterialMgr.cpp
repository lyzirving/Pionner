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
		m_materials.insert({ GpuAttr::Mat::STANDARD, Material::Create(GpuAttr::Mat::STANDARD, ShaderSpec_Standard)});
		m_materials.insert({ GpuAttr::Mat::SPRITE, Material::Create(GpuAttr::Mat::SPRITE, ShaderSpec_Sprite, RenderingMode_Overlay) });
		m_materials.insert({ GpuAttr::Mat::COLOR, Material::Create(GpuAttr::Mat::COLOR, ShaderSpec_Color, RenderingMode_Overlay) });
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

	Ref<Material> MaterialMgr::create(const std::string& name, ShaderSpecifier spec, RenderingMode mode)
	{		
		return Material::Create(name, spec, mode);
	}
}