#ifndef __PIONNER_SCENE_RESOURCES_MATERIAL_H__
#define __PIONNER_SCENE_RESOURCES_MATERIAL_H__

#include "asset/Asset.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class UniformData;

	class Material : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Material)
	public:
		Material(const std::string &name = "");
		Material(ShaderSpecifier spec, RenderingMode mode, const std::string &name = "");
		virtual ~Material() = default;

		void setShaderSpec(ShaderSpecifier spec) { m_shaderSpec = spec; }
		void setRenderingMode(RenderingMode mode) { m_renderingMode = mode; }

		ShaderSpecifier shaderSpec() const { return m_shaderSpec; }
		RenderingMode renderingMode() const { return m_renderingMode; }
		const std::string& name() const { return m_name; }
		const std::map<std::string, Ref<UniformData>>& uniforms() const { return m_uniforms; }

	protected:
		ShaderSpecifier m_shaderSpec{ ShaderSpec_Standard };
		RenderingMode m_renderingMode{ RenderingMode_Opaque };
		std::map<std::string, Ref<UniformData>> m_uniforms;
		std::string m_name;
	};

	template<>
	bool Asset::is<Material>() const;
}

#endif