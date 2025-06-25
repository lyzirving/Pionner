#include "ImageHDR.h"

#include "base/utils/ImageUtil.h"

#include "gfx/rhi/Texture.h"
#include "gfx/pass/CubeMapBuildPass.h"
#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ImageHDR"

namespace pio
{
	ImageHDR::ImageHDR(const ImportParams& params) : Image(params)
	{
	}

	void ImageHDR::OnInit()
	{
		if(m_Texture)
			return;

		if(!m_Data)
		{
			LOGE("[%s] byte data hasn't been parsed", m_Params.Name.c_str());
			return;
		}		

		if(m_Params.Setting.Image.InternalType == ImageInternal_Equirectangular)
		{
			BuildFromEquiAngular();
		}
		else
		{
			LOGE("internal type[%u] hasn't been implemented", m_Params.Setting.Image.InternalType);
			std::abort();
		}		
	}

	void ImageHDR::BuildFromEquiAngular()
	{
		auto context = m_Context.lock();
		if(!context)
		{
			LOGE("[%s] context is invalid", m_Params.Name.c_str());
			return;
		}		

		TextureSpecificBuilder builder;
		builder.Name(std::string(Name()).append("-Equirectangular"))
			.Type(TextureType::TwoDimen)
			.Format(TextureFormat::RGB_16F)
			.TexWrap(m_TextureParams.WrapS, m_TextureParams.WrapT)
			.TexFilter(m_TextureParams.MinFilter, m_TextureParams.MagFilter)
			.Width(m_Width).Height(m_Height)
			.Channel(m_Comp);
		m_SourceTexture = context->CreateTexture(builder.Build(), &m_Data);
	
		m_ToolPass = CreateRef<CubeMapBuildPass>(Self<ImageHDR>(), "CubeMapBuildTool");
		m_ToolPass->OnAttach(context);
		m_ToolPass->OnExecute(context, Ref<CameraNode>());
	}
}