#ifndef __PIONNER_SCENE_RESOURCES_MATERIAL_H__
#define __PIONNER_SCENE_RESOURCES_MATERIAL_H__

#include "asset/Asset.h"
#include "gfx/GfxDef.h"

namespace pio
{
	class UniformData;
	class Shader;

	class Material : public Asset
	{
		OVERRIDE_ASSET_TYPE(AssetType::Material)
	public:
		Material(const std::string& name, RenderingMode mode, ShaderSpecifier spec);
		virtual ~Material() = default;
		virtual void bind(Ref<Shader>& shader) = 0;

		void setSpec(ShaderSpecifier spec) { m_spec = spec; }
		void setRenderingMode(RenderingMode mode) { m_renderingMode = mode; }

		ShaderSpecifier spec() const { return m_spec; }
		RenderingMode renderingMode() const { return m_renderingMode; }
		const std::string& name() const { return m_name; }
		const std::map<std::string, Ref<UniformData>>& uniforms() const { return m_uniforms; }

	public:
		static Ref<Material> MakeStandardMaterial(const std::string& name, RenderingMode mode = RenderingMode_Opaque);
		static Ref<Material> Create(const std::string& name, RenderingMode mode, ShaderSpecifier spec);

	protected:
		std::string m_name;		
		RenderingMode m_renderingMode{ RenderingMode_Opaque };
		ShaderSpecifier m_spec{ ShaderSpec_Standard };
		std::map<std::string, Ref<UniformData>> m_uniforms;		
	};

	template<>
	bool Asset::is<Material>() const;
}

#endif