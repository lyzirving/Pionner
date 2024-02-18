#ifndef __PIONNER_ASSET_MATERIAL_IMPORTER_H__
#define __PIONNER_ASSET_MATERIAL_IMPORTER_H__

#include "core/Base.h"

namespace pio
{
	class Material;

	enum class MaterialTextureType : uint8_t
	{
		Albedo
	};

	class MaterialImporter
	{
	public:
		MaterialImporter(const std::string &name);
		MaterialImporter(const std::string &path, const std::string &name);

		Ref<Material> importMaterial();
		Ref<Material> importColorMaterial();

	private:
		static void Parse(Ref<Material> &material, const std::string &materialName, const std::string &parentDir, const std::string &fileName);
		
		static bool IsAlbedo(const std::string &fileName) {	return StringUtil::contains(fileName, "albedo") || StringUtil::contains(fileName, "baseColor") || StringUtil::contains(fileName, "diffuse"); }
		static bool IsNormal(const std::string &fileName) { return StringUtil::contains(fileName, "normal"); }
		static bool IsMetalness(const std::string &fileName) { return StringUtil::contains(fileName, "metallic"); }
		static bool IsRoughness(const std::string &fileName) { return StringUtil::contains(fileName, "roughness"); }
		static bool IsAO(const std::string &fileName) { return StringUtil::contains(fileName, "ao"); }

		static bool ValidPostfix(const char *postfix) { return std::strcmp(postfix, ".png") == 0 || std::strcmp(postfix, ".jpg") == 0 || std::strcmp(postfix, ".jpeg") == 0; }

	private:
		std::string m_path, m_name;
	};
}

#endif