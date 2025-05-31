#include "MaterialImporter.h"

#include "asset/AssetMgr.h"
#include "asset/image/Image.h"
#include "asset/material/StandardMaterial.h"
#include "asset/material/TexturedMaterial.h"

#include "base/utils/AssimpUtils.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MaterialImporter"

namespace pio
{
	MaterialImporter::MaterialImporter(const ImportParams& params) : Importer(params)
	{
	}

	Ref<Asset> MaterialImporter::Load()
	{
		if (m_ImportParams.Setting.Mat.AiMat)
		{
			LOGD("load material from assimp data");
			switch (m_ImportParams.Setting.Mat.Type)
			{
				case MaterialFormat_Standard:
					return ImportIMPL_Std_Assimp(m_ImportParams);
				default:
					LOGE("err! no loader for type[%U]",
						 m_ImportParams.Setting.Mat.Type);
					return Ref<Asset>();
			}
		}
		else
		{
			return ImportDefaultMaterial(m_ImportParams);
		}
	}

	Ref<Asset> MaterialImporter::ImportDefaultMaterial(const ImportParams& params)
	{
		switch (params.Setting.Mat.Type)
		{
			case MaterialFormat_Standard:
			{
				return ImportIMPL_Std_Default(params);
			}
			case MaterialFormat_Textured:
			{
				return ImportIMPL_Textured(params);
			}
			default:
				LOGE("err! material type[%u] hasn't been supported");
				std::abort();
				return Ref<Asset>();
		}
	}

	Ref<Asset> MaterialImporter::ImportIMPL_Std_Assimp(const ImportParams& params)
	{
		auto context = params.Context.lock();
		auto white = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::WHITE));
		auto black = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::BLACK));

		auto* AIMaterial = params.Setting.Mat.AiMat;

		auto material = CreateRef<StandardMaterial>(params);
		LOGD("begin to load std material[%s] from assimp", material->FullPath().c_str());

		//default
		glm::vec3 albedoColor{ 0.7f }, emission{ 0.f };
		float roughness{ 0.5f }, metalness{ 0.f };
		// 0(fully transparent) -- 1(fully opaque)
		float opacity{ 1.f };

		aiColor3D aiColor{};
		ai_real aiFloat{ 0.f };
		aiString aiTexPath;

		if (AIMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
			albedoColor = { aiColor.r, aiColor.g, aiColor.b };

		if (AIMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiColor) == AI_SUCCESS)
			emission = glm::vec3(aiColor.r, aiColor.g, aiColor.b);

		if (AIMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, aiFloat) == AI_SUCCESS)
			roughness = aiFloat;

		if (AIMaterial->Get(AI_MATKEY_METALLIC_FACTOR, aiFloat) == AI_SUCCESS)
			metalness = aiFloat;

		if (AIMaterial->Get(AI_MATKEY_OPACITY, aiFloat) == AI_SUCCESS)
			opacity = aiFloat;

		material->SetAlbedo(albedoColor);
		material->SetEmission(emission);
		material->SetMetallic(metalness);
		material->SetRoughness(roughness);
		material->SetAlpha(opacity);

		// ------------------------ Base Color Start --------------------------------		
		bool hasAlbedoMap = AIMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
		bool fallback = !hasAlbedoMap;
		if (hasAlbedoMap)
		{
			ImportParams curParams;
			curParams.SetFmt(AssetFormat_Image)
				.SetContext(context)
				.SetPath(params.Path)
				.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
				.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
				.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
							 TextureFilterMin::Linear, TextureFilterMag::Linear));
			auto img = AssetMgr::Get()->LoadAsset<Image>(curParams);			
			if (img)
			{
				LOGD("suceed to parse base color[%s]", img->FullPath());
				material->SetAlbedoMap(img);
			}
			else
			{
				LOGE("fail to base color tex[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}

		if (fallback)
		{
			LOGW("no base color map, use fallback[%s]", aiTexPath.C_Str());
			material->SetAlbedoMap(white);
		}
		// ------------------------ Base Color End ----------------------------------

		// ------------------------- Emission Start ---------------------------------		
		bool hasEmission = (AIMaterial->GetTexture(aiTextureType_EMISSIVE, 0, &aiTexPath) == AI_SUCCESS) ||
			(AIMaterial->GetTexture(aiTextureType_EMISSION_COLOR, 0, &aiTexPath) == AI_SUCCESS);
		fallback = !hasEmission;
		if (hasEmission)
		{
			ImportParams curParams;
			curParams.SetFmt(AssetFormat_Image)
				.SetContext(context)
				.SetPath(params.Path)
				.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
				.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
				.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
							 TextureFilterMin::Linear, TextureFilterMag::Linear));
			auto img = AssetMgr::Get()->LoadAsset<Image>(curParams);
			if (img)
			{
				LOGD("suceed to parse emission[%s]", img->FullPath());
				material->SetEmissionMap(img);
			}
			else
			{
				LOGE("fail to load emission tex[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}

		if (fallback)
		{
			LOGW("no emission tex, use fallback[%s]", aiTexPath.C_Str());
			material->SetEmissionMap(black);
		}
		// ------------------------- Emission End ---------------------------------

		// ------------------------- NormalMap Start ------------------------------		
		bool hasNormalMap = AIMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
		fallback = !hasNormalMap;
		if (hasNormalMap)
		{
			ImportParams curParams;
			curParams.SetFmt(AssetFormat_Image)
				.SetContext(context)
				.SetPath(params.Path)
				.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
				.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
				.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
							 TextureFilterMin::Linear, TextureFilterMag::Linear));
			auto img = AssetMgr::Get()->LoadAsset<Image>(curParams);
			if (img)
			{
				LOGD("suceed to parse normal map[%s]", img->FullPath());
				material->SetNormalMap(img);
			}
			else
			{
				LOGE("fail to load normal map[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}

		if (fallback)
		{
			LOGW("no normal map, use fallback[%s]", aiTexPath.C_Str());
		}
		// ------------------------- NormalMap End --------------------------------

		// ------------------------- MetallicRoughness Start ------------------------------
		// r channel: ao, g channel: roughness, b channel:metallic
		Ref<Image> metalicRoughnessImg{ nullptr };
		bool hasMetallicMap = AIMaterial->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &aiTexPath) == AI_SUCCESS;
		bool hasMetallicRoughness = hasMetallicMap && StringUtil::Contains(aiTexPath.C_Str(), "roughness");
		fallback = !hasMetallicMap && !hasMetallicRoughness;
		if (hasMetallicRoughness)
		{
			ImportParams curParams;
			curParams.SetFmt(AssetFormat_Image)
				.SetContext(context)
				.SetPath(params.Path)
				.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
				.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
				.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
							 TextureFilterMin::Linear, TextureFilterMag::Linear));
			auto img = AssetMgr::Get()->LoadAsset<Image>(curParams);
			if (img)
			{
				LOGD("suceed to parse metallic-roughness[%s]", img->FullPath());
				material->SetMetallicRoughness(img);
			}
			else
			{
				LOGE("fail to load metallicRoughness[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}
		else if (hasMetallicMap)
		{
			int32_t w{ 0 }, h{ 0 }, c{ 0 };
			uint8_t* data{ nullptr };
			if (ImageUtil::LoadImage(aiTexPath.C_Str(), &data, w, h, c))
			{
				//roughness is marked 1.f as default
				uint8_t* imgData = ImageUtil::CreateImageData(w, h, 3, 255);
				//fetch metallic from B channel and fill it into B channel
				int32_t srcReq = c >= 3 ? 3 : 1;
				ImageUtil::FillChannelData(data, c, srcReq, imgData, 3, 3, w, h);
				ImageUtil::FreeImage(&data);

				ImportParams curParams;
				curParams.SetFmt(AssetFormat_Image)
					.SetContext(context)
					.SetPath(params.Path)
					.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
					.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
					.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
								 TextureFilterMin::Linear, TextureFilterMag::Linear))
					.SetImageData(w, h, c, &imgData)
					.SetActivateOnLoad(false);
				metalicRoughnessImg = AssetMgr::Get()->LoadAsset<Image>(curParams);
			}
			else
			{
				LOGE("fail to load metallic map[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}

		bool hasRoughnessMap = AIMaterial->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &aiTexPath) == AI_SUCCESS;
		if ((fallback || (hasMetallicMap && !hasMetallicRoughness)) && hasRoughnessMap)
		{
			int32_t w{ 0 }, h{ 0 }, c{ 0 };
			uint8_t* data{ nullptr };
			if (ImageUtil::LoadImage(aiTexPath.C_Str(), &data, w, h, c))
			{
				int32_t srcReq = c >= 3 ? 2 : 1;
				uint8_t* imgData{ nullptr };
				if (metalicRoughnessImg)
				{
					imgData = metalicRoughnessImg->Data();
					ImageUtil::FillChannelData(data, c, srcReq, imgData, 3, 2, w, h);
				}
				else
				{
					//make metallic 0 for default
					imgData = ImageUtil::CreateImageData(w, h, 3, 0);
					ImageUtil::FillChannelData(data, c, srcReq, imgData, 3, 2, w, h);
					ImportParams curParams;
					curParams.SetFmt(AssetFormat_Image)
						.SetContext(context)
						.SetPath(params.Path)
						.SetName(Path::PathWithoutSuffix(aiTexPath.C_Str()))
						.SetSuffix(Path::FindSuffix(aiTexPath.C_Str()))
						.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge,
									 TextureFilterMin::Linear, TextureFilterMag::Linear))
						.SetImageData(w, h, c, &imgData);
					metalicRoughnessImg = AssetMgr::Get()->LoadAsset<Image>(curParams);
				}
				ImageUtil::FreeImage(&data);
			}
			else
			{
				LOGE("fail to load roughness map[%s]", aiTexPath.C_Str());
				fallback = true;
			}
		}

		if (metalicRoughnessImg)
		{
			LOGD("suceed to make metallic-roughness[%s]", metalicRoughnessImg->FullPath());
			metalicRoughnessImg->OnInit();
			material->SetMetallicRoughness(metalicRoughnessImg);
		}
		else if (fallback)
		{
			LOGW("no metallic-roughness tex, use fallback[%s]", aiTexPath.C_Str());
			material->SetMetallicRoughness(white);
		}

		return material;
	}

	Ref<Asset> MaterialImporter::ImportIMPL_Std_Default(const ImportParams& params)
	{
		auto context = params.Context.lock();
		auto white = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::WHITE));
		auto black = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::BLACK));

		auto material = CreateRef<StandardMaterial>(params);
		glm::vec3 albedoColor{ 0.7f }, emission{ 0.f };
		float roughness{ 0.6f }, metalness{ 0.f };
		float opacity{ 1.f };

		material->SetAlbedo(albedoColor);
		material->SetEmission(emission);
		material->SetMetallic(metalness);
		material->SetRoughness(roughness);
		material->SetAlpha(opacity);

		material->SetAlbedoMap(white);
		material->SetOcclustionMap(white);
		material->SetEmissionMap(black);
		material->SetMetallicRoughness(white);

		return material;
	}

	Ref<Asset> MaterialImporter::ImportIMPL_Textured(const ImportParams& params)
	{		
		auto material = CreateRef<TexturedMaterial>(params);
		auto white = AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::WHITE));
		material->SetTexture(white);		
		return material;
	}
}