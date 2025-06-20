#pragma once

#include "Common.h"

namespace pio
{
	namespace ImageAsset
	{
		constexpr const char* WHITE = "white";
		constexpr const char* BLACK = "black";
		constexpr const char* ICON_POINT_LIT = "point_light";
		constexpr const char* IBL = "ibl";
		constexpr const char* IBL_SPECULAR = "ibl_specular";
	};

	enum ImageFormat : uint8_t
	{
		ImageFormat_None,
		ImageFormat_BITMAP,
		ImageFormat_PNG,
		ImageFormat_JPG,
		ImageFormat_JPEG
	};

	enum class TextureWrap : uint8_t
	{
		ClampEdge,
		ClampBorder,
		Repeat
	};

	enum class TextureFilterMin : uint8_t
	{
		Nearest,
		Linear,
		Linear_Mipmap_Linear,
		Linear_Mipmap_Nearest,
		Nearest_Mipmap_Linear
	};

	enum class TextureFilterMag : uint8_t
	{
		Nearest,
		Linear
	};

	enum class TextureFormat : uint8_t
	{
		RGBA_32,    //8 bits per channel
		RGB_24,     //8 bits per channel
		RGB_16,     //integer values in the range [0, 65535] per channel
		R_16,       //integer values in the range [0, 65535] per channel
		R_8,
		Alpha_8,
		RGBA_FLOAT,
		RGBA_HALF,  //16 bits floating point per channel
		RG_FLOAT,
		R_FLOAT,
		DEPTH_16,	//16 bits for a single channel depth map
		DEPTH_24,   //24 bits for a single channel depth map
		DEPTH_32,   //32 bits for a single channel depth map
		DEPTH_32F,  //32 bits as high resolution float for a single channel depth map
		DEPTH_24_STENCIL_8 // 24 bits for depth channel and 8 bits for stencil channel
	};

	struct TextureParams
	{
		TextureWrap WrapS{ TextureWrap::ClampEdge };
		TextureWrap WrapT{ TextureWrap::ClampEdge };
		TextureFilterMin MinFilter{ TextureFilterMin::Nearest };
		TextureFilterMag MagFilter{ TextureFilterMag::Nearest };

		TextureParams() {}
		TextureParams(TextureWrap s, TextureWrap t, TextureFilterMin min, TextureFilterMag mag) : 
			WrapS(s), WrapT(t), MinFilter(min), MagFilter(mag)
		{
		}

		bool operator==(const TextureParams& params) const
		{
			return WrapS == params.WrapS && WrapT == params.WrapT
				&& MinFilter == params.MinFilter
				&& MagFilter == params.MagFilter;
		}

		bool operator !=(const TextureParams& params) const
		{
			return !((*this) == params);
		}
	};

	uint32_t GetTextureChannelNum(TextureFormat fmt);
	uint32_t GetTextureByteSize(TextureFormat fmt);
	ImageFormat GetFormatByChannal(int32_t comp);
}