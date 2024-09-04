#include "PbrMaterial.h"

#include "gfx/rhi/Shader.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "PbrMaterial"

namespace pio
{
	PbrMaterial::PbrMaterial(const std::string& name, RenderingMode mode)
		: Material(name, mode, ShaderSpec_Standard)
	{
	}

	void PbrMaterial::bind(Ref<Shader>& shader)
	{
	}

	template<>
	bool Asset::is<PbrMaterial>() const
	{
		if (is<Material>())
		{
			return as<Material>()->spec() == ShaderSpec_Standard;
		}
		return false;
	}
}