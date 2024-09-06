#include "Texture.h"

#include "asset/AssetMgr.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/opengl/GLTexture2D.h"
#include "gfx/rhi/opengl/GLRenderBuffer.h"

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

	RenderBuffer::RenderBuffer(Ref<RenderContext>& context, const TextureSpecific& spec)
		: Texture(context, spec, RenderResourceType::RenderBuffer)
	{
		m_spec.Channel = Rhi::GetTextureChannelNum(spec.Format);
		m_size = m_spec.Width * m_spec.Height * m_spec.Channel * Rhi::GetTextureByteSize(spec.Format);
	}

	Ref<Texture> Texture::Create(Ref<RenderContext>& context, const TextureSpecific& spec)
	{
		switch (context->renderBackend())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch (spec.Type)
				{
					case TextureType::TwoDimen:
					case TextureType::SingleChannel:
					case TextureType::NormalMap:
					{
						return AssetMgr::MakeRuntimeAsset<GLTexture2D>(context, spec);
					}
					case TextureType::RenderBuffer:
					{
						return CreateRef<GLRenderBuffer>(context, spec);
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
				LOGE("err! render backend[%u] has not been implemented", context->renderBackend());
				std::abort();
				return Ref<Texture>();
			}
		}
	}

	Ref<Texture> Texture::Create(Ref<RenderContext>& context, const TextureSpecific& spec, Buffer& buffer)
	{
		switch(context->renderBackend())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch(spec.Type)
				{
					case TextureType::TwoDimen:
					case TextureType::SingleChannel:
					case TextureType::NormalMap:
					{
						return AssetMgr::MakeRuntimeAsset<GLTexture2D>(context, spec, buffer);
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
				LOGE("err! render backend[%u] has not been implemented", context->renderBackend());
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
					type == TextureType::NormalMap);
		}
		return false;
	}

	template<>
	bool Asset::is<RenderBuffer>() const { return is<Texture>() && (as<Texture>()->spec().Type == TextureType::RenderBuffer); }
}