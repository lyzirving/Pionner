#include "Texture.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLTexture2D.h"
#include "gfx/rhi/opengl/GLCubeMap.h"
#include "gfx/rhi/opengl/GLCubeMapArray.h"
#include "gfx/rhi/opengl/GLRenderBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Texture"

namespace pio
{
	Texture2D::Texture2D(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: Texture(context, spec)
	{
		m_Spec.Channel = GetTextureChannelNum(spec.Format);
		m_Size = m_Spec.Width * m_Spec.Height * m_Spec.Channel * 
			    GetTextureByteSize(spec.Format) * m_Spec.Num;
	}	

	RenderBuffer::RenderBuffer(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: Texture(context, spec, RenderResourceType::RenderBuffer)
	{
		m_Spec.Channel = GetTextureChannelNum(spec.Format);
		m_Size = m_Spec.Width * m_Spec.Height * m_Spec.Channel * 
			     GetTextureByteSize(spec.Format) * m_Spec.Num;
	}

	CubeMap::CubeMap(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: Texture2D(context, spec)
	{
	}

	CubeMapArray::CubeMapArray(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: CubeMap(context, spec)
	{
	}

	Ref<Texture> Texture::Create(const Ref<RenderContext>& context, const TextureSpecific& spec)
	{
		switch (context->RenderBackend())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch (spec.Type)
				{
					case TextureType::TwoDimen:
					case TextureType::SingleChannel:
					case TextureType::NormalMap:
					{
						return CreateRef<GLTexture2D>(context, spec);
					}
					case TextureType::CubeMap:
					{
						return CreateRef<GLCubeMap>(context, spec);
					}
					case TextureType::CubeMapArray:
					{
						return CreateRef<GLCubeMapArray>(context, spec);
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
				LOGE("err! render backend[%u] has not been implemented", context->RenderBackend());
				std::abort();
				return Ref<Texture>();
			}
		}
	}

	Ref<Texture> Texture::Create(const Ref<RenderContext>& context, const TextureSpecific& spec, Buffer& buffer)
	{
		switch(context->RenderBackend())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch(spec.Type)
				{
					case TextureType::TwoDimen:
					case TextureType::SingleChannel:
					case TextureType::NormalMap:
					{
						return CreateRef<GLTexture2D>(context, spec, buffer);
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
				LOGE("err! render backend[%u] has not been implemented", context->RenderBackend());
				std::abort();
				return Ref<Texture>();
			}
		}
	}

	Ref<Texture> Texture::Create(const Ref<RenderContext>& context, const TextureSpecific& spec, void** buffer)
	{
		switch(context->RenderBackend())
		{
			case RenderBackendFlags::RenderBackend_OpenGL:
			{
				switch(spec.Type)
				{
					case TextureType::TwoDimen:
					case TextureType::SingleChannel:
					case TextureType::NormalMap:
					{
						return CreateRef<GLTexture2D>(context, spec, buffer);
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
				LOGE("err! render backend[%u] has not been implemented", context->RenderBackend());
				std::abort();
				return Ref<Texture>();
			}
		}
	}
}