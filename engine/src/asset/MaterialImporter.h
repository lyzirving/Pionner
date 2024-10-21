#ifndef __PIONNER_ASSET_MATERIAL_IMPORTER_H__
#define __PIONNER_ASSET_MATERIAL_IMPORTER_H__

#include "gfx/GfxDef.h"

namespace pio
{
	class Material;
	class RenderContext;

	class MaterialImporter
	{
	public:
		static bool IsAlbedo(const std::string& fileName);
		static bool IsNormal(const std::string& fileName);
		static bool IsMetalness(const std::string& fileName);
		static bool IsGlossiness(const std::string& fileName);
		static bool IsMetallicGlossiness(const std::string& fileName);
		static bool IsEmission(const std::string& fileName);
		static bool IsAO(const std::string& fileName);

	public:
		MaterialImporter(Ref<RenderContext>& context, ShaderSpecifier spec, const std::string &name);

		Ref<Material> getMaterial() const { return m_material; }

	private:		
		void load();

	private:
		static void ParsePbr(Ref<RenderContext>& context, Ref<Material>& material, const std::string& parentDir, const std::string& fileName);

	private:
		ShaderSpecifier m_spec;
		Ref<RenderContext> m_context;
		Ref<Material> m_material;

		std::string m_name, m_path;		
	};
}

#endif