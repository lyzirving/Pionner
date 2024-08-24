#ifndef __PIONNER_SCENE_RESOURCES_MATERIAL_H__
#define __PIONNER_SCENE_RESOURCES_MATERIAL_H__

#include "asset/Asset.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class Material : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Material)
	public:
		Material();
		Material(ShaderSpecifier spec, RenderingMode mode);
		virtual ~Material() = default;

		void setShaderSpec(ShaderSpecifier spec) { m_shaderSpec = spec; }
		void setRenderingMode(RenderingMode mode) { m_renderingMode = mode; }

		ShaderSpecifier shaderSpec() const { return m_shaderSpec; }
		RenderingMode renderingMode() const { return m_renderingMode; }

	private:
		ShaderSpecifier m_shaderSpec{ ShaderSpec_Standard };
		RenderingMode m_renderingMode{ RenderingMode_Opaque };
	};

	template<>
	bool Asset::is<Material>() const;
}

#endif