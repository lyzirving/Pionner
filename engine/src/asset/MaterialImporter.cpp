#include <io.h>
#include <stdio.h>

#include "MaterialImporter.h"

#include "asset/AssetsManager.h"

#include "gfx/rhi/Material.h"
#include "gfx/rhi/Texture.h"

#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialImporter"

namespace pio
{
	MaterialImporter::MaterialImporter(const std::string &name) : m_name(name)
	{
	}

	MaterialImporter::MaterialImporter(const std::string &path, const std::string &name)
		: m_path(path), m_name(name)
	{
	}

	Ref<Material> MaterialImporter::importMaterial()
	{
		intptr_t fileHandle{ -1 };
		// file information
		struct _finddata_t fileInfo;
		Ref<Material> material;

		std::string p = m_path + "/*";

		if ((fileHandle = _findfirst(p.c_str(), &fileInfo)) != -1)
		{
			do
			{
				if (!(fileInfo.attrib & _A_SUBDIR)) // find a file
				{
					const char *occur = std::strchr(fileInfo.name, '.');
					if (occur && ValidPostfix(occur))
						Parse(material, m_name, m_path, fileInfo.name);
				}
			} while (_findnext(fileHandle, &fileInfo) == 0);// if success, _findnext() return 0.
		}

		if (material)
		{
			auto emission = material->getTexture2D(MaterialAttrs::MU_EmissionTexture);
			if (!emission)
			{
				material->set(MaterialAttrs::MU_EmissionTexture, Renderer::GetBlackTexture());
			}
		}

		return material;
	}

	Ref<Material> MaterialImporter::importColorMaterial()
	{
		Ref<Material> material = Material::Create(m_name);
		Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();
		Ref<Texture2D> backTexture = Renderer::GetBlackTexture();

		material->set(MaterialAttrs::MU_AlbedoTexture, whiteTexture);
		material->set(MaterialAttrs::MU_AlbedoColor, glm::vec3(1.f));

		material->set(MaterialAttrs::MU_MetalnessTexture, whiteTexture);
		material->set(MaterialAttrs::MU_Metalness, 0.f);

		material->set(MaterialAttrs::MU_RoughnessTexture, whiteTexture);
		material->set(MaterialAttrs::MU_Roughness, 0.f);

		material->set(MaterialAttrs::MU_AOTexture, whiteTexture);
		material->set(MaterialAttrs::MU_AO, 1.f);

		material->set(MaterialAttrs::MU_EmissionTexture, backTexture);
		material->set(MaterialAttrs::MU_Emission, 0.f);

		return material;
	}

	void MaterialImporter::Parse(Ref<Material> &material, const std::string &materialName, const std::string &parentDir, const std::string &fileName)
	{
		std::string name(fileName.substr(0, fileName.find_first_of('.')));
		std::string path = parentDir + "/" + fileName;
		Ref<Texture2D> whiteTexture = Renderer::GetWhiteTexture();

		if (IsAlbedo(fileName))
		{
			if (!material)
				material = Material::Create(materialName);

			TextureSpecification spec;
			spec.Name = name;
			spec.SRGB = true;
			spec.Format = ImageInternalFormat::FROM_FILE;
			Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);

			if (texture)
			{
				material->set(MaterialAttrs::MU_AlbedoTexture, texture);
				material->set(MaterialAttrs::MU_AlbedoColor, glm::vec3(1.f));
			}
			else
			{
				material->set(MaterialAttrs::MU_AlbedoTexture, whiteTexture);
				material->set(MaterialAttrs::MU_AlbedoColor, glm::vec3(0.f));
			}
		}
		else if (IsMetalness(fileName))
		{
			if (!material)
				material = Material::Create(materialName);

			TextureSpecification spec;
			spec.Name = name;
			spec.SRGB = true;
			spec.Format = ImageInternalFormat::FROM_FILE;
			Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);

			if (texture)
			{
				material->set(MaterialAttrs::MU_MetalnessTexture, texture);
				material->set(MaterialAttrs::MU_Metalness, 1.f);
			}
			else
			{
				material->set(MaterialAttrs::MU_MetalnessTexture, whiteTexture);
				material->set(MaterialAttrs::MU_Metalness, 0.f);
			}
		}
		else if (IsRoughness(fileName))
		{
			if (!material)
				material = Material::Create(materialName);

			TextureSpecification spec;
			spec.Name = name;
			spec.SRGB = true;
			spec.Format = ImageInternalFormat::FROM_FILE;
			Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);

			if (texture)
			{
				material->set(MaterialAttrs::MU_RoughnessTexture, texture);
				material->set(MaterialAttrs::MU_Roughness, 1.f);
			}
			else
			{
				material->set(MaterialAttrs::MU_RoughnessTexture, whiteTexture);
				material->set(MaterialAttrs::MU_Roughness, 0.5f); // default roughness
			}
		}
		else if (IsAO(fileName))
		{
			if (!material)
				material = Material::Create(materialName);

			TextureSpecification spec;
			spec.Name = name;
			spec.SRGB = true;
			spec.Format = ImageInternalFormat::FROM_FILE;
			Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);

			if (texture)
			{
				material->set(MaterialAttrs::MU_AOTexture, texture);
				material->set(MaterialAttrs::MU_AO, 1.f);
			}
			else
			{
				material->set(MaterialAttrs::MU_AOTexture, whiteTexture);
				material->set(MaterialAttrs::MU_AO, 1.f);
			}
		}
		// TODO: if we parse normal map, we should manually calculate the tangent.
		/*else if (IsNormal(fileName))
		{
			if (!material)
				material = Material::Create(materialName);

			TextureSpecification spec;
			spec.Name = name;
			spec.SRGB = true;
			spec.Format = ImageInternalFormat::FROM_FILE;
			Ref<Texture2D> texture = AssetsManager::GetOrCreatePackedAsset<Texture2D>(path, spec);

			if (texture)
			{
				material->set(MaterialAttrs::MU_NormalTexture, texture);
				material->set(MaterialAttrs::MU_UseNormalMap, true);
			}
			else
			{
				material->set(MaterialAttrs::MU_NormalTexture, whiteTexture);
				material->set(MaterialAttrs::MU_UseNormalMap, false);
			}
		}*/
		//std::string absPath = parentDir + "/" + fileName;
	}
}