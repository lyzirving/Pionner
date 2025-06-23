#include "Image.h"

#include "base/utils/ImageUtil.h"

#include "gfx/rhi/Texture.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Image"

namespace pio
{
	Image::Image(const ImportParams& params) : Asset(params)
	{
		if(!m_Params.Setting.Image.bUseData)
		{
			m_Format = ParseFormat(m_FullPath);
		}
	}

	void Image::OnInit()
	{
		if(m_Texture)
			return;

		if(!m_Data)
		{
			LOGE("[%s] byte data hasn't been parsed", m_Params.Name.c_str());
			return;
		}		

		auto context = m_Context.lock();
		if(!context)
		{
			LOGE("[%s] context is invalid", m_Params.Name.c_str());
			return;
		}

		TextureSpecificBuilder builder;
		builder.Name(m_Key)
			.Type(TextureType::TwoDimen)
			.Format(ParseTextureFmt(m_Comp))
			.TexWrap(m_TextureParams.WrapS, m_TextureParams.WrapT)
			.TexFilter(m_TextureParams.MinFilter, m_TextureParams.MagFilter)
			.Width(m_Width).Height(m_Height)
			.Channel(m_Comp);
		m_Texture = context->CreateTexture(builder.Build(), &m_Data);
	}

	void Image::SetData(void** data, int32_t w, int32_t h, int32_t comp)
	{
		if(data && m_Data != *data)
		{
			ImageUtil::FreeImage(&m_Data);
			m_Data = *data;
			m_Width = w;
			m_Height = h;
			m_Comp = comp;
			if(m_Data)
			{
				*data = nullptr;
			}
		}
	}

	ImageFormat Image::ParseFormat(const std::string& fullPath)
	{
		auto suffix = Path::FindSuffix(fullPath);
		if(suffix == Path::PNG_SUFFIX)
		{
			return ImageFormat_PNG;
		}
		else if(suffix == Path::JPEG_SUFFIX)
		{
			return ImageFormat_JPEG;
		}
		else if(suffix == Path::JPG_SUFFIX)
		{
			return ImageFormat_JPG;
		}
		else if(suffix == Path::HDR_SUFFIX)
		{
			return ImageFormat_HDR;
		}
		else
		{
			LOGE("fail to parse suffix for image[%s]", fullPath.c_str());
			return ImageFormat_None;
		}
	}

	TextureFormat Image::ParseTextureFmt(int32_t comp)
	{
		switch(comp)
		{
			case 4:
				return TextureFormat::RGBA_32;
			case 3:
				return TextureFormat::RGB_24;
			case 1:
				return TextureFormat::R_8;
			default:
				LOGE("invalid comp[%d]", comp);
				std::abort();
				return TextureFormat::R_8;
		}
	}
}