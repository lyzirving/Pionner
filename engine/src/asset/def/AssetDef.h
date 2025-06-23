#pragma once

#include "ImageDef.h"
#include "MaterialDef.h"
#include "MeshDef.h"
#include "SkeletalDef.h"

namespace pio
{
	class RenderContext;

	enum AssetFormat : uint8_t
	{
		AssetFormat_None,
		AssetFormat_Image,
		AssetFormat_Mesh,
		AssetFormat_Material,
		AssetFormat_Skeleton,
		AssetFormat_AnimClip,
		AssetFormat_Num
	};

	struct ImportSettings
	{
		struct
		{
			ImageInternalType InternalType{ ImageInternal_Pixel };
			TextureParams Param{};
			int32_t Width{ 0 }, Height{ 0 };
			int32_t Comp{ 0 };
			uint8_t* Data{ nullptr };
			bool bUseData{ false };
		}Image;

		struct
		{
			MaterialFormat Type{ MaterialFormat_None };
			aiMaterial* AiMat{ nullptr };
		}Mat;

		struct
		{
			SubMesh InitMesh{};
			bool bUseMesh{ false };
		}Mesh;

		ImportSettings() {}

		ImportSettings(const ImportSettings& rhs)
		{
			this->Image = rhs.Image;
			this->Mat = rhs.Mat;
			this->Mesh = rhs.Mesh;
		}

		ImportSettings(ImportSettings&& rhs) noexcept
		{
			this->Image = std::move(rhs.Image);
			this->Mat = std::move(rhs.Mat);
			this->Mesh = std::move(rhs.Mesh);
		}

		ImportSettings& operator=(const ImportSettings& rhs)
		{
			if (this != &rhs)
			{
				this->ImportSettings::ImportSettings(rhs);
			}
			return *this;
		}

		ImportSettings& operator=(ImportSettings&& rhs) noexcept
		{
			if (this != &rhs)
			{
				this->ImportSettings::ImportSettings(std::move(rhs));
			}
			return *this;
		}
	};

	struct ImportParams
	{
		WeakRef<RenderContext> Context;
		ImportSettings Setting{};
		std::string Path{};
		std::string Name{};
		std::string Suffix{};
		AssetFormat Format{ AssetFormat_None };
		bool ActivateOnLoad{ true };

		ImportParams& SetFmt(AssetFormat fmt)
		{
			this->Format = fmt;
			return *this;
		}

		ImportParams& SetPath(const std::string& path)
		{
			this->Path = path;
			return *this;
		}

		ImportParams& SetName(const std::string& name)
		{
			this->Name = name;
			return *this;
		}

		ImportParams& SetSuffix(const std::string& suffix)
		{
			this->Suffix = suffix;
			return *this;
		}

		ImportParams& SetImageData(int32_t w, int32_t h, int32_t comp, uint8_t** data)
		{
			this->Setting.Image.Width = w;
			this->Setting.Image.Height = h;
			this->Setting.Image.Comp = comp;
			this->Setting.Image.Data = *data;
			this->Setting.Image.bUseData = true;
			*data = nullptr;
			return *this;
		}

		ImportParams& SetImageInternalType(ImageInternalType type)
		{
			this->Setting.Image.InternalType = type;
			return *this;
		}

		ImportParams& SetChannelNum(uint32_t num)
		{
			this->Setting.Image.Comp = num;
			return *this;
		}

		ImportParams& SetTexParam(const TextureParams& param)
		{
			this->Setting.Image.Param = param;
			return *this;
		}

		ImportParams& SetActivateOnLoad(bool val)
		{
			this->ActivateOnLoad = val;
			return *this;
		}

		ImportParams& SetContext(const Ref<RenderContext>& context)
		{
			this->Context = context;
			return *this;
		}

		ImportParams& SetSubMesh(SubMesh&& iMesh)
		{
			this->Setting.Mesh.InitMesh = std::move(iMesh);
			this->Setting.Mesh.bUseMesh = true;
			return *this;
		}

		ImportParams& SetMatSpec(MaterialFormat format)
		{
			this->Setting.Mat.Type = format;
			return *this;
		}

		ImportParams& SetMatPtr(aiMaterial* material)
		{
			this->Setting.Mat.AiMat = material;
			return *this;
		}
	};
}