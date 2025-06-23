#include "AssetMgr.h"

#include "asset/importer/Importer.h"
#include "asset/mesh/MeshFactory.h"
#include "asset/mesh/StaticMesh.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssetMgr"

namespace pio
{
	PIO_SINGLETON_IMPL(AssetMgr)

	void AssetMgr::Init(const InitParam& param)
	{
		AssetMgr::Get()->m_Context = param.Context;

		//@note create default asssets in main thread

		uint8_t* whiteData = ImageUtil::CreateImageData(1, 1, 3, 255);
		ImportParams whiteImgParam;
		whiteImgParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::WHITE)
			.SetSuffix(Path::JPG_SUFFIX)
			.SetContext(param.Context)
			.SetImageData(1, 1, 3, &whiteData);
		AssetMgr::Get()->LoadAsset(whiteImgParam);

		uint8_t* blackData = ImageUtil::CreateImageData(1, 1, 3, 0);
		ImportParams blackImgParam;
		blackImgParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::BLACK)
			.SetSuffix(Path::JPG_SUFFIX)
			.SetContext(param.Context)
			.SetImageData(1, 1, 3, &blackData);
		AssetMgr::Get()->LoadAsset(blackImgParam);

		ImportParams pointLitIconParam;
		pointLitIconParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::ICON_POINT_LIT)
			.SetSuffix(Path::PNG_SUFFIX)
			.SetContext(param.Context);
		AssetMgr::Get()->LoadAsset(pointLitIconParam);

		ImportParams iblParam;
		iblParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::IBL)
			.SetSuffix(Path::PNG_SUFFIX)
			.SetContext(param.Context);
		AssetMgr::Get()->LoadAsset(iblParam);

		ImportParams iblSpecularParam;
		iblSpecularParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::IBL_SPECULAR)
			.SetSuffix(Path::PNG_SUFFIX)
			.SetContext(param.Context);
		AssetMgr::Get()->LoadAsset(iblSpecularParam);

		ImportParams lightRoomParam;
		lightRoomParam.SetFmt(AssetFormat_Image)
			.SetPath(Path::ImageRoot())
			.SetName(ImageAsset::LIGHT_ROOM)
			.SetSuffix(Path::HDR_SUFFIX)
			.SetImageInternalType(ImageInternal_Equirectangular)
			.SetTexParam(TextureParams(TextureWrap::ClampEdge, TextureWrap::ClampEdge, TextureFilterMin::Linear, TextureFilterMag::Linear))
			.SetContext(param.Context);
		AssetMgr::Get()->LoadAsset(lightRoomParam);

		ImportParams matParam;
		matParam.SetFmt(AssetFormat_Material)
			.SetPath(Path::MaterialRoot())
			.SetName(MatAsset::STANDARD)
			.SetSuffix(Path::MAT_SUFFIX)
			.SetContext(param.Context)
			.SetMatSpec(MaterialFormat_Standard);
		AssetMgr::Get()->LoadAsset(matParam);

		ImportParams texturedMatParam;
		texturedMatParam.SetFmt(AssetFormat_Material)
			.SetPath(Path::MaterialRoot())
			.SetName(MatAsset::TEXTURED)
			.SetSuffix(Path::MAT_SUFFIX)
			.SetContext(param.Context)
			.SetMatSpec(MaterialFormat_Textured);
		AssetMgr::Get()->LoadAsset(texturedMatParam);

		ImportParams planeParam;
		planeParam.SetFmt(AssetFormat_Mesh)
			.SetPath(Path::MeshRoot())
			.SetName(MeshAsset::PLANE)
			.SetSuffix(Path::MESH_SUFFIX)
			.SetContext(param.Context)
			.SetSubMesh(MeshFactory::MakePlane())
			.SetMatSpec(MaterialFormat_Standard);
		AssetMgr::Get()->LoadAsset(planeParam);

		ImportParams cubeParam;
		cubeParam.SetFmt(AssetFormat_Mesh)
			.SetPath(Path::MeshRoot())
			.SetName(MeshAsset::CUBE)
			.SetSuffix(Path::MESH_SUFFIX)
			.SetContext(param.Context)
			.SetSubMesh(MeshFactory::MakeCube())
			.SetMatSpec(MaterialFormat_Standard);
		AssetMgr::Get()->LoadAsset(cubeParam);

		ImportParams sphereParam;
		sphereParam.SetFmt(AssetFormat_Mesh)
			.SetPath(Path::MeshRoot())
			.SetName(MeshAsset::SPHERE)
			.SetSuffix(Path::MESH_SUFFIX)
			.SetContext(param.Context)
			.SetSubMesh(MeshFactory::MakeSphere())
			.SetMatSpec(MaterialFormat_Standard);
		AssetMgr::Get()->LoadAsset(sphereParam);

		ImportParams dirLitGizmoParam;
		dirLitGizmoParam.SetFmt(AssetFormat_Mesh)
			.SetPath(Path::MeshRoot())
			.SetName(MeshAsset::DIRLIT_GIZMO)
			.SetSuffix(Path::MESH_SUFFIX)
			.SetContext(param.Context)
			.SetSubMesh(MeshFactory::MakeDirLightGizmo(0.5f, 2.f))
			.SetMatSpec(MaterialFormat_Textured);
		AssetMgr::Get()->LoadAsset(dirLitGizmoParam);

		ImportParams pointLitGizmoParam;
		pointLitGizmoParam.SetFmt(AssetFormat_Mesh)
			.SetPath(Path::MeshRoot())
			.SetName(MeshAsset::POINTLIT_GIZMO)
			.SetSuffix(Path::MESH_SUFFIX)
			.SetContext(param.Context)
			.SetSubMesh(MeshFactory::MakePointLightGizmo(2.f))
			.SetMatSpec(MaterialFormat_Textured);
		AssetMgr::Get()->LoadAsset(pointLitGizmoParam);
	}

	void AssetMgr::Shutdown()
	{
		AssetMgr::Get()->RemoveAll();
	}

	Ref<Asset> AssetMgr::LoadAsset_Impl(const ImportParams& param)
	{
		auto result = Importer::Build(param)->Load();
		if(param.ActivateOnLoad && result)
			result->Init();

		return result;
	}
}