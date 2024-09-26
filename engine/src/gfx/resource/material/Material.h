#ifndef __PIONNER_GFX_RESOURCES_MATERIAL_MATERIAL_H__
#define __PIONNER_GFX_RESOURCES_MATERIAL_MATERIAL_H__

#include "asset/Asset.h"

#include "gfx/GfxDef.h"
#include "gfx/rhi/UniformData.h"

namespace pio
{
	class RenderContext;
	class Texture;
	class Shader;

	class Material : public Asset
	{
		PIO_DEFINE_ASSET_TYPE(AssetType::Material)
	public:
		Material(const std::string& name, RenderingMode mode, ShaderSpecifier spec);
		Material(const Material& rhs);
		Material& operator=(const Material& rhs);

		virtual ~Material() = default;
		virtual void update(Ref<RenderContext>& context) = 0;

		void bind(Ref<Shader>& shader);

		void setSpec(ShaderSpecifier spec) { m_spec = spec; }
		void setRenderingMode(RenderingMode mode) { m_renderingMode = mode; }

		ShaderSpecifier spec() const { return m_spec; }
		RenderingMode renderingMode() const { return m_renderingMode; }
		const std::string& name() const { return m_name; }
		const std::map<std::string, Ref<UniformData>>& uniforms() const { return m_uniforms; }

	private:
		friend class MaterialMgr;
		static Ref<Material> Create(const std::string& name, ShaderSpecifier spec, RenderingMode mode = RenderingMode_Opaque);

	protected:
		template<typename T>
		void updateUnimData(const std::string& name, const T& data)
		{
			auto it = m_uniforms.find(name);
			if (it != m_uniforms.end())
			{
				it->second->write(&data);
			}
			else
			{
				auto unimData = UniformData::Create<T>(name);
				unimData->write(&data);
				m_uniforms.insert({ name, unimData });
			}
		}

		void updateTexture(const std::string& name, Ref<Texture>& texture)
		{
			auto it = m_textures.find(name);
			if (it == m_textures.end())
			{
				m_textures.insert({ name, texture });
			}
			else if (it->second != texture)
			{
				m_textures[name] = texture;
			}
		}

	protected:
		std::string m_name;		
		RenderingMode m_renderingMode{ RenderingMode_Opaque };
		ShaderSpecifier m_spec{ ShaderSpec_Standard };

		std::map<std::string, Ref<UniformData>> m_uniforms;
		std::map<std::string, Ref<Texture>> m_textures;
	};

	template<>
	bool Asset::is<Material>() const;
}

#endif