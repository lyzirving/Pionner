#include <io.h>
#include <stdio.h>

#include "MaterialImporter.h"
#include "AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/resource/material/PbrMaterial.h"
#include "gfx/rhi/Texture.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

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
			   StringUtil::Contains(fileName, "diffuse"); 	
	}

	bool MaterialImporter::IsNormal(const std::string& fileName)
	{ 
		return StringUtil::Contains(fileName, "normal"); 
	}

	bool MaterialImporter::IsMetalness(const std::string& fileName)
	{ 
		return StringUtil::Contains(fileName, "metallic"); 
	}

	bool MaterialImporter::IsGlossiness(const std::string& fileName)
	{
		return StringUtil::Contains(fileName, "glossiness");
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

	void MaterialImporter::ParsePbr(Ref<RenderContext>& context, Ref<Material>& material, const std::string& parentDir, const std::string& fileName)
	{
		std::string name(fileName.substr(0, fileName.find_first_of('.')));
		std::string path = parentDir + "/" + fileName;

		auto* pbrMat = material->as<PbrMaterial>();
		auto whiteTex = context->getTexture(GpuAttr::Tex::WHITE);
		auto blackTex = context->getTexture(GpuAttr::Tex::BLACK);

		if (MaterialImporter::IsMetalness(fileName))
		{
			int32_t w, h, comp;
			uint8_t* data;
			/*if (!pbrMat->m_metallicRoughness)
			{				
				data = stbi_load(path.c_str(), &w, &h, &comp, 0);

				TextureSpecificBuilder builder;
				builder.name(name)
					   .type(TextureType::TwoDimen)
					   .format(TextureFormat::RGB_24);
				Buffer buff;
				buff.allocate(0, w * h * 3);
				pbrMat->m_metallicRoughness = context->createTexture(builder.build(), buff);
			}*/
		}
		else if (MaterialImporter::IsAlbedo(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setAlbedoTexture(texture); }
			pbrMat->setAlbedo(glm::vec3(1.f));
		}
		else if (MaterialImporter::IsAO(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::SingleChannel);
			auto texture = context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setAoTexture(texture); }
			pbrMat->setOcclustion(1.f);
		}
		else if (MaterialImporter::IsNormal(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setNormalMap(texture); }
		}
		else if (MaterialImporter::IsEmission(fileName))
		{
			TextureSpecificBuilder builder;
			builder.name(name).type(TextureType::TwoDimen);
			auto texture = context->createTexture(builder.build(), path);
			if (texture) { pbrMat->setEmissionTexture(texture); }
			pbrMat->setEmission(glm::vec3(1.f));
		}
		else
		{
			LOGW("warning! file[%s] can't be parsed for pbr material", fileName.c_str());
		}
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
								ParsePbr(m_context, m_material, m_path, fileInfo.name);
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
}