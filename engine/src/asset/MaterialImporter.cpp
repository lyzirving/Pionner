#include <io.h>
#include <stdio.h>

#include "MaterialImporter.h"
#include "AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/material/PbrMaterial.h"
#include "gfx/rhi/Texture.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialImporter"

namespace pio
{
	static bool ValidPostfix(const char* postfix)
	{
		return std::strcmp(postfix, ".png") == 0 ||
			   std::strcmp(postfix, ".jpg") == 0 ||
			   std::strcmp(postfix, ".jpeg") == 0;
	}

	bool MaterialImporter::IsAlbedo(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "albedo") ||
			   StringUtil::Contains(fileName, "baseColor") ||
			   StringUtil::Contains(fileName, "diffuse") ||
			   StringUtil::Contains(fileName, "diff");
	}

	bool MaterialImporter::IsNormal(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "normal") ||
			   StringUtil::Contains(fileName, "nor");
	}

	bool MaterialImporter::IsHeight(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "height") ||
			   StringUtil::Contains(fileName, "disp");
	}

	bool MaterialImporter::IsMetalness(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "metallic");
	}

	bool MaterialImporter::IsGlossiness(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "glossiness");
	}

	bool MaterialImporter::IsRoughness(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "roughness");
	}

	bool MaterialImporter::IsMetallicRoughness(const std::string& fileName)
	{
		return IsMetalness(fileName) && IsRoughness(fileName);
	}

	bool MaterialImporter::IsMetallicGlossiness(const std::string& fileName)
	{
		return IsMetalness(fileName) && IsGlossiness(fileName);
	}

	bool MaterialImporter::IsEmission(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "emission");
	}

	bool MaterialImporter::IsAO(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "ao");
	}

	MaterialImporter::MaterialImporter(Ref<RenderContext>& context, ShaderSpecifier spec, const std::string& name)
		: m_spec(spec), m_context(context),
		m_name(name), m_path(AssetMgr::MaterialPath(name))
	{
		load();
	}

	void MaterialImporter::load()
	{
		std::string p = m_path;
		p.append(FileUtil::SEPARATOR).append("*");

		intptr_t fileHandle{ -1 };
		// file information
		struct _finddata_t fileInfo;

		if ((fileHandle = _findfirst(p.c_str(), &fileInfo)) != -1)
		{
			do
			{
				if (!(fileInfo.attrib & _A_SUBDIR)) // find a file
				{
					const char* occur = std::strchr(fileInfo.name, '.');
					if (occur && ValidPostfix(occur))
					{
						switch (m_spec)
						{
							case ShaderSpec_Standard:
							{
								if (!m_material)
								{
									m_material = m_context->createMaterial(m_name, m_spec);
									m_material->as<PbrMaterial>()->setEmissionTexture(m_context->getTexture(GpuAttr::Tex::BLACK));
									m_material->as<PbrMaterial>()->setEmission(glm::vec3(1.f));
								}
								parsePbr(m_path, fileInfo.name);
								break;
							}
							default:
								LOGE("err! shader spec has not been implemented");
								std::abort();
								break;
						}
					}
				}
			} while (_findnext(fileHandle, &fileInfo) == 0);// if success, _findnext() return 0.
		}
	}

	void MaterialImporter::parsePbr(const std::string& parentDir, const std::string& fileName)
	{
		std::string name(fileName.substr(0, fileName.find_first_of('.')));
		std::string path = parentDir + "/" + fileName;

		auto* pbrMat = m_material->as<PbrMaterial>();

		if (MaterialImporter::IsMetalness(fileName))
		{
			createMetallicRoughnessIfNeed(pbrMat, path, fileName);
			int32_t w{ 0 }, h{ 0 }, comp{ 0 };
			void* data{ nullptr };
			if (!pbrMat->m_metallicRoughness)
			{
				LOGE("err! metallic-roughness tex is invalid for metalness parsing");
				return;
			}

			if (!ImageUtil::LoadImage(path, &data, w, h, comp))
			{
				LOGE("err! fail to load image[%s]", path.c_str());
				return;
			}

			void* texData = pbrMat->m_metallicRoughness->as<Texture2D>()->data();
			// Green channel for roughness, blue channel for metalness			
			if (IsMetallicRoughness(fileName))
			{				
				// @todo If the image is layout in bgra
				ImageUtil::FillChannelData((uint8_t*)data, 3, 1, (uint8_t*)texData, 3, 1, w, h);//roughness
				ImageUtil::FillChannelData((uint8_t*)data, 3, 2, (uint8_t*)texData, 3, 2, w, h);//metalness
				pbrMat->setMetallic(1.f);
				pbrMat->setRoughness(1.f);
			}
			else if (IsMetallicGlossiness(fileName))
			{
				ImageUtil::FillChannelData((uint8_t*)data, 4, 0, (uint8_t*)texData, 3, 2, w, h);//metalness
				ImageUtil::FillChannelData((uint8_t*)data, 4, 3, (uint8_t*)texData, 3, 1, w, h, true, (uint8_t)255);//roughness
				pbrMat->setMetallic(1.f); 
				pbrMat->setRoughness(1.f);
			}
			else
			{
				ImageUtil::FillChannelData((uint8_t*)data, 1, 0, (uint8_t*)texData, 3, 2, w, h);//metalness
				pbrMat->setMetallic(1.f);				
			}
			ImageUtil::FreeImage(&data);
		}
		else if (MaterialImporter::IsRoughness(fileName))
		{
			createMetallicRoughnessIfNeed(pbrMat, path, fileName);
			int32_t w{ 0 }, h{ 0 }, comp{ 0 };
			void* data{ nullptr };
			if (!pbrMat->m_metallicRoughness)
			{
				LOGE("err! metallic-roughness tex is invalid for roughness parsing");
				return;
			}

			if (!ImageUtil::LoadImage(path, &data, w, h, comp))
			{
				LOGE("err! fail to load image[%s]", path.c_str());
				return;
			}

			void* texData = pbrMat->m_metallicRoughness->as<Texture2D>()->data();
			ImageUtil::FillChannelData((uint8_t*)data, 1, 0, (uint8_t*)texData, 3, 1, w, h);
			ImageUtil::FreeImage(&data);
			pbrMat->setRoughness(1.f);			
		}
		else if (MaterialImporter::IsAO(fileName))
		{
			createMetallicRoughnessIfNeed(pbrMat, path, fileName);
			int32_t w{ 0 }, h{ 0 }, comp{ 0 };
			void* data{ nullptr };
			if (!pbrMat->m_metallicRoughness)
			{
				LOGE("err! metallic-roughness tex is invalid for ao parsing");
				return;
			}
			
			if (!ImageUtil::LoadImage(path, &data, w, h, comp))
			{
				LOGE("err! fail to load image[%s]", path.c_str());
				return;
			}
			void* texData = pbrMat->m_metallicRoughness->as<Texture2D>()->data();
			ImageUtil::FillChannelData((uint8_t*)data, 1, 0, (uint8_t*)texData, 3, 0, w, h);
			ImageUtil::FreeImage(&data);
			pbrMat->setOcclustion(1.f);
		}
		else if (MaterialImporter::IsAlbedo(fileName))
		{			
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = m_context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setAlbedoTexture(texture); }
			pbrMat->setAlbedo(glm::vec3(1.f));
		}		
		else if (MaterialImporter::IsNormal(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = m_context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setNormalMap(texture); }
		}
		else if (MaterialImporter::IsEmission(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = m_context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setEmissionTexture(texture); }
			pbrMat->setEmission(glm::vec3(1.f));
		}
		else
		{
			LOGW("warning! file[%s] can't be parsed for pbr material", fileName.c_str());
		}
	}

	void MaterialImporter::createMetallicRoughnessIfNeed(PbrMaterial* pbrMat, const std::string& path, const std::string& name)
	{
		int32_t w{ 0 }, h{ 0 }, comp{ 0 };
		if (!pbrMat->m_metallicRoughness)
		{
			if (ImageUtil::GetPicInfo(path.c_str(), w, h, comp))
			{
				Buffer buff;
				buff.allocate(255, w * h * 3);//r(ao)|g(roughness)|b(metallic)
				void* data = buff.data();
				ImageUtil::FillSingleChannel((uint8_t*)data, 3, 0, w, h, (uint8_t)0);
				TextureSpecificBuilder builder;
				auto texName = name;
				texName.append("-MetallicRoughness");
				builder.name(texName)
					   .type(TextureType::TwoDimen)
					   .format(TextureFormat::RGB_24)
					   .width(w).height(h)
					   .texFilter(TextureFilterMin::Nearest, TextureFilterMag::Nearest);
				pbrMat->m_metallicRoughness = m_context->createTexture(builder.build(), buff);
			}
			else
			{
				LOGE("err! fail to get pic info for image[%s]", path.c_str());
			}
		}
	}
}