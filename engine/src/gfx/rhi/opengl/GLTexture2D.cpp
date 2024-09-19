#include "GLTexture2D.h"

#include "gfx/rhi/opengl/GLHelper.h"
#include "gfx/rhi/opengl/GLHeader.h"

#ifndef STB_IMAGE_STATIC
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_STATIC
#include <stb/stb_image.h>

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLTexture2D"

namespace pio
{
	GLTexture2D::GLTexture2D(Ref<RenderContext>& context, const TextureSpecific& spec) : Texture2D(context, spec)
	{
	}

	GLTexture2D::GLTexture2D(Ref<RenderContext>& context, const std::string& path, const TextureSpecific& spec) : Texture2D(context, spec)
	{
		auto startTime = Time::CurrentTimeMs();
		if (m_spec.FlipVerticalWhenLoad)
			stbi_set_flip_vertically_on_load(true);//Flip the image, let the data start from left-bottom

		m_data = stbi_load(path.c_str(), &m_spec.Width, &m_spec.Height, &m_spec.Channel, 0);
		if (m_data)
		{
			m_size = m_spec.Width * m_spec.Height * m_spec.Channel;
			switch (m_spec.Channel)
			{
				case 3:
					m_spec.Format = TextureFormat::RGB_24;
					break;
				case 4:
					m_spec.Format = TextureFormat::RGBA_32;
					break;
				case 1:
				default:
					m_spec.Format = TextureFormat::R_8;
					break;
			}
			LOGD("load image[%s], size[%u, %u], channel[%u], cost time[%lu]ms", path.c_str(), m_spec.Width, m_spec.Height, m_spec.Channel, (Time::CurrentTimeMs() - startTime));
		}

		if (m_spec.FlipVerticalWhenLoad)
			stbi_set_flip_vertically_on_load(false);
	}

	GLTexture2D::GLTexture2D(Ref<RenderContext>& context, const TextureSpecific& spec, Buffer& buff) : Texture2D(context, spec)
	{
		buff.move(&m_data);
	}

	bool GLTexture2D::init()
	{
		if (!isInit())
		{
			glGenTextures(1, &m_id);

			glBindTexture(GL_TEXTURE_2D, m_id);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLHelper::GetTextureMinFilter(m_spec.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLHelper::GetTextureMagFilter(m_spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLHelper::GetTextureWrap(m_spec.WrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLHelper::GetTextureWrap(m_spec.WrapT));
			glTexImage2D(GL_TEXTURE_2D, 0,
						 GLHelper::GetTextureInternalFmt(m_spec.Format),
						 m_spec.Width, m_spec.Height, 0,
						 GLHelper::GetTexturePixelFmt(m_spec.Format),
						 GLHelper::GetTexturePixelType(m_spec.Format),
						 m_data);

			float borderColor[] = { m_spec.BorderColor.x, m_spec.BorderColor.y, m_spec.BorderColor.z, m_spec.BorderColor.w };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			if (m_spec.GenerateMips)
				glGenerateMipmap(GL_TEXTURE_2D);

			if (GLHelper::CheckError("err! fail to init texture2D[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init texture2D[%s][%u]", m_spec.Name.c_str(), m_id);

			glBindTexture(GL_TEXTURE_2D, 0);
			
			if (m_data)
			{
				std::free(m_data);
				m_data = nullptr;
			}
		}
		return isInit();
	}

	void GLTexture2D::destroy()
	{
		if (isInit())
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	void GLTexture2D::bind()
	{
		init();

		if (isInit())
		{
			glBindTexture(GL_TEXTURE_2D, m_id);
			GLHelper::CheckError("fail to bind texture[%s][%u]", m_spec.Name.c_str(), m_id);
		}
		else
		{
			LOGE("err! texture[%s] has not been initilized", m_spec.Name.c_str());
		}
	}

	void GLTexture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::active(TextureSampler sampler)
	{
		if (sampler == TextureSampler::MaxSlotNum || 
			sampler == TextureSampler::InvalidSlot)
		{
			LOGE("err! invalid sampler[%u]", sampler);
			return;
		}

		init();

		if (isInit())
		{
			glActiveTexture(GL_TEXTURE0 + PIO_UINT8(sampler));
		}
		else
		{
			LOGE("err! texture[%s] has not been initilized", m_spec.Name.c_str());
		}
	}
}