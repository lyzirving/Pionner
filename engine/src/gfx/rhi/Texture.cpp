#include "Texture.h"

#include "asset/AssetMgr.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLTexture2D.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Texture"

namespace pio
{
	Texture2D::Texture2D(Ref<RenderContext>& context, const TextureSpecific& spec)
		: Texture(context, spec)
	{
		m_spec.Channel = Rhi::GetTextureChannelNum(spec.Format);
		m_size = m_spec.Width * m_spec.Height * m_spec.Channel * Rhi::GetTextureByteSize(spec.Format);
	}

	Ref<Texture> Texture::Create(Ref<RenderContext>& context, const TextureSpecific& spec)
	{
		switch (context->backendFlag())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch (spec.Type)
				{
					case TextureType::TwoDimen:
					{
						return AssetMgr::MakeRuntimeAsset<GLTexture2D>(context, spec);
					}
					default:
					{
						LOGE("err! texture type[%u] has not been implemented", spec.Type);
						std::abort();
						return Ref<Texture>();
					}
				}
			}
			default:
			{
				LOGE("err! render backend[%u] has not been implemented", context->backendFlag());
				std::abort();
				return Ref<Texture>();
			}
		}
	}

	template<>
	bool Asset::is<Texture>() const { return assetType() == AssetType::Texture; }

	template<>
	bool Asset::is<Texture2D>() const
	{
		if (is<Texture>())
		{
			auto type = as<Texture>()->spec().Type;
			return (type == TextureType::TwoDimen ||
					type == TextureType::SingleChannel ||
					type == TextureType::NormalMap ||
					type == TextureType::RenderBuffer);
		}
		return false;
	}
		                                    
}