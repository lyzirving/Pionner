#include "ShadowMapFrameBuffer.h"

#include "GlobalSettings.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CascadeShadowFrameBuffer"

namespace pio
{
	ShadowMapFrameBuffer::ShadowMapFrameBuffer(Ref<RenderContext>& context) 
		: FrameBufferProxy(context, "ShadowMap")
	{
		auto shadowReso = GlobalSettings::ShadowResolution();
		initInner(context, shadowReso.x, shadowReso.y);
	}

	ShadowMapFrameBuffer::ShadowMapFrameBuffer(Ref<RenderContext>& context, uint32_t width, uint32_t height) 
		: FrameBufferProxy(context, "ShadowMap")
	{
		initInner(context, width, height);
	}

	void ShadowMapFrameBuffer::initInner(Ref<RenderContext>& context, uint32_t width, uint32_t height)
	{
		FrameBufferSpecific fboSpec;
		fboSpec.Name = "ShadowMap";
		fboSpec.Width = width;
		fboSpec.Height = height;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Depth);

		TextureSpecificBuilder depth;
		depth.name("ShadowMapDepthBuffer")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::DEPTH_32F)
			.width(width).height(height)
			.texWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.border(glm::vec4(1.f))
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);
		fboSpec.DepthSpec.push_back(depth.build());

		m_frameBuff = FrameBuffer::Create(context, fboSpec);
	}

	CascadeShadowFrameBuffer::CascadeShadowFrameBuffer(Ref<RenderContext>& context) 
		: FrameBufferProxy(context, "CascadeShadowMap")
	{
		auto shadowReso = GlobalSettings::ShadowResolution();
		initInner(context, shadowReso.x, shadowReso.y);
	}

	CascadeShadowFrameBuffer::CascadeShadowFrameBuffer(Ref<RenderContext>& context, uint32_t width, uint32_t height) 
		: FrameBufferProxy(context, "CascadeShadowMap")
	{
		initInner(context, width, height);
	}

	void CascadeShadowFrameBuffer::initInner(Ref<RenderContext>& context, uint32_t width, uint32_t height)
	{
		FrameBufferSpecific fboSpec;
		fboSpec.Name = "CascadeShadowMap";
		fboSpec.Width = width;
		fboSpec.Height = height;
		PIO_FBO_ADD_USAGE(fboSpec.Usage, FrameBufferUsage_Depth);

		TextureSpecificBuilder lev0;
		lev0.name("CascadeShadowLevel_0")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::DEPTH_32F)
			.width(width).height(height)
			.texWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.border(glm::vec4(1.f))
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);
		fboSpec.DepthSpec.push_back(lev0.build());

		TextureSpecificBuilder lev1;
		lev1.name("CascadeShadowLevel_1")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::DEPTH_32F)
			.width(width).height(height)
			.texWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.border(glm::vec4(1.f))
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);
		fboSpec.DepthSpec.push_back(lev1.build());

		TextureSpecificBuilder lev2;
		lev2.name("CascadeShadowLevel_2")
			.type(TextureType::TwoDimen)
			.format(TextureFormat::DEPTH_32F)
			.width(width).height(height)
			.texWrap(TextureWrap::ClampBorder, TextureWrap::ClampBorder)
			.border(glm::vec4(1.f))
			.texFilter(TextureFilterMin::Linear, TextureFilterMag::Linear);
		fboSpec.DepthSpec.push_back(lev2.build());

		m_frameBuff = FrameBuffer::Create(context, fboSpec);
	}	
}