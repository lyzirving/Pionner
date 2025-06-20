#include "IndirectLight.h"
#include "RenderContext.h"

#include "GlobalSettings.h"

#include "gfx/rhi/Texture.h"
#include "gfx/rhi/Shader.h"
#include "gfx/rhi/Texture.h"

#include "asset/AssetMgr.h"
#include "asset/image/Image.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndirectLight"

namespace pio
{
	IndirectLight::IndirectLight(const Ref<RenderContext>& context, const std::string& name)
		: FrameBufferProxy(context, name)
	{
		m_IblDFG = RefCast<Texture, Texture2D>(AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::IBL))->GetTexture());
		m_IblSpecular = RefCast<Texture, Texture2D>(AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::IBL_SPECULAR))->GetTexture());
		m_SSR = RefCast<Texture, Texture2D>(AssetMgr::Get()->GetAsset<Image>(Path::ImageKey(ImageAsset::WHITE))->GetTexture());		
	}

	void IndirectLight::OnTick()
	{
	}

	void IndirectLight::BindAt(const Ref<Shader>& shader)
	{
		m_IblDFG->BindAt(shader, GpuAttr::UNI_IBL_DFG);
		m_IblSpecular->BindAt(shader, GpuAttr::UNI_IBL_SPECULAR);
		m_SSR->BindAt(shader, GpuAttr::UNI_IBL_SSR);
	}

	void IndirectLight::UnBindAt()
	{
		m_IblDFG->UnBind();
		m_IblSpecular->UnBind();
		m_SSR->UnBind();
	}
}
