#include "GLTexture.h"
#include "GLHelper.h"

#include "gfx/renderer/RenderData.h"
#include "asset/StbUtils.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLTexture"

namespace pio
{
	GLTexture::GLTexture(const TextureSpecification &spec) : m_spec(spec)
	{
		m_spec.Channel = Image::GetChannelCount(m_spec.Format);
		m_internalFmt = Image::GetInternalFmt(m_spec.Format);
		m_textureFmt = Image::GetTextureFmt(m_spec.Format);
		m_pixelType = Image::GetPixelType(m_spec.Format);
		m_size = m_spec.Width * m_spec.Height * m_spec.Channel;
	}

	GLTexture2D::GLTexture2D(const std::string &path, const TextureSpecification &spec)
		: Texture2D(), GLTexture()/*use GLTexture() to ignore invalod log print*/
	{
		m_spec = spec;
		uint64_t start = TimeUtil::CurrentTimeMs();
		if(spec.FlipVerticalWhenLoad)
			stbi_set_flip_vertically_on_load(true);
		uint8_t *data = stbi_load(path.c_str(), &m_spec.Width, &m_spec.Height, &m_spec.Channel, 0);
		if (data)
		{
			m_size = m_spec.Width * m_spec.Height * m_spec.Channel;
			m_imageData = CreateRef<Buffer>(data, m_size);
			m_internalFmt = RhiDef::GetInternalFmt(m_spec.Channel);
			m_textureFmt = RhiDef::GetTextureFmt(m_spec.Channel);
			m_pixelType = PixelType::UNSIGNED_BYTE;
			LOGD("load image[%s], cost time[%lu]ms", path.c_str(), (TimeUtil::CurrentTimeMs() - start));
		}
		else
		{
			LOGE("fail to load image[%s]", path.c_str());
		}
		if (spec.FlipVerticalWhenLoad)
			stbi_set_flip_vertically_on_load(false);
	}

	GLTexture2D::GLTexture2D(const TextureSpecification &spec, const Ref<Buffer> &buffer)
		: Texture2D(), GLTexture(spec), m_imageData(buffer)
	{				
		m_size = m_imageData->getCapacity();
	}

	GLTexture2D::GLTexture2D(const TextureSpecification &spec) : Texture2D(), GLTexture(spec)
	{
	}

	GLTexture2D::~GLTexture2D()
	{
		destroy();
		m_imageData.reset();
	}

	bool GLTexture2D::init()
	{
		if (!isInit())
		{
			glGenTextures(1, &m_textureId);

			glBindTexture(GL_TEXTURE_2D, m_textureId);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLHelper::GetGLTextureMinFilter(m_spec.MinFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLHelper::GetGLTextureMagFilter(m_spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLHelper::GetGLTextureWrap(m_spec.WrapS));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLHelper::GetGLTextureWrap(m_spec.WrapT));
			glTexImage2D(GL_TEXTURE_2D, 0, 
						 GLHelper::GetGLTextureInternalFmt(m_internalFmt), 
						 m_spec.Width, m_spec.Height, 0, 
						 GLHelper::GetGLTextureFmt(m_textureFmt), 
						 GLHelper::GetGLTexturePixelType(m_pixelType), 
						 m_imageData.get() ? m_imageData->as<void *>() : nullptr);

			float borderColor[] = { m_spec.BorderColor.x, m_spec.BorderColor.y, m_spec.BorderColor.z, m_spec.BorderColor.w };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			if (m_spec.GenerateMips)
				glGenerateMipmap(GL_TEXTURE_2D);

			if (GLHelper::CheckError("err happens when init texture2D[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init texture2D[%s][%u]", m_spec.Name.c_str(), m_textureId);

			glBindTexture(GL_TEXTURE_2D, 0);
			if(m_imageData) m_imageData->release();
		}
		return isInit();
	}

	void GLTexture2D::destroy()
	{
		if (isInit())
		{
			glDeleteTextures(1, &m_textureId);
			m_textureId = 0;
		}
	}

	void GLTexture2D::active(uint32_t sampler)
	{
		init();

		if (isInit())
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
		}
		else
		{
			LOGE("texture2D[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLTexture2D::bind()
	{
		init();

		if (isInit())
		{
			glBindTexture(GL_TEXTURE_2D, m_textureId);
			GLHelper::CheckError("fail to bind texture[%s][%u]", m_spec.Name.c_str(), m_textureId);
		}
		else
		{
			LOGE("texture2D[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLTexture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLCubeTexture::GLCubeTexture(const TextureSpecification &spec) : CubeTexture(), GLTexture(spec)
	{
	}

	GLCubeTexture::~GLCubeTexture()
	{
		destroy();
	}

	bool GLCubeTexture::init()
	{
		if (!isInit())
		{
			glGenTextures(1, &m_textureId);

			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);	

			// NOTE: Cube Texture's width should be equal to its height
			for (uint8_t i = 0; i < LightDir_Num; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
							 GLHelper::GetGLTextureInternalFmt(m_internalFmt),
							 m_spec.Width, m_spec.Height, 0,
							 GLHelper::GetGLTextureFmt(m_textureFmt),
							 GLHelper::GetGLTexturePixelType(m_pixelType), nullptr);
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GLHelper::GetGLTextureMinFilter(m_spec.MinFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GLHelper::GetGLTextureMagFilter(m_spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GLHelper::GetGLTextureWrap(m_spec.WrapS));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GLHelper::GetGLTextureWrap(m_spec.WrapT));
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GLHelper::GetGLTextureWrap(m_spec.WrapR));

			if (GLHelper::CheckError("err happens when init cube texture[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init cube texture[%s][%u], size[%u, %u]", m_spec.Name.c_str(), m_textureId, m_spec.Width, m_spec.Height);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		return isInit();
	}

	void GLCubeTexture::destroy()
	{
		if (isInit())
		{
			glDeleteTextures(1, &m_textureId);
			m_textureId = 0;
		}
	}

	void GLCubeTexture::active(uint32_t sampler)
	{
		init();

		if (isInit())
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
		}
		else
		{
			LOGE("cube texture[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLCubeTexture::bind()
	{
		init();

		if (isInit())
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
			GLHelper::CheckError("fail to bind cube texture[%s][%u]", m_spec.Name.c_str(), m_textureId);
		}
		else
		{
			LOGE("cube texture[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLCubeTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	GLCubeArrayTexture::GLCubeArrayTexture(const TextureSpecification &spec) : CubeArrayTexture(), GLTexture(spec)
	{
	}

	GLCubeArrayTexture::~GLCubeArrayTexture()
	{
		destroy();
	}

	bool GLCubeArrayTexture::init()
	{
		if (!isInit())
		{
			glGenTextures(1, &m_textureId);

			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_textureId);

			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GLHelper::GetGLTextureMinFilter(m_spec.MinFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GLHelper::GetGLTextureMagFilter(m_spec.MaxFilter));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GLHelper::GetGLTextureWrap(m_spec.WrapS));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GLHelper::GetGLTextureWrap(m_spec.WrapT));
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GLHelper::GetGLTextureWrap(m_spec.WrapR));
			
			glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0,
						 GLHelper::GetGLTextureInternalFmt(m_internalFmt),
						 m_spec.Width, m_spec.Height, m_spec.Num * 6, 0,
						 GLHelper::GetGLTextureFmt(m_textureFmt), 
						 GLHelper::GetGLTexturePixelType(m_pixelType), nullptr);

			if (GLHelper::CheckError("err happens when init cube array texture[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init texture cube array texture[%s][%u]", m_spec.Name.c_str(), m_textureId);

			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
		}
		return isInit();
	}

	void GLCubeArrayTexture::destroy()
	{
		if (isInit())
		{
			glDeleteTextures(1, &m_textureId);
			m_textureId = 0;
		}
	}

	void GLCubeArrayTexture::active(uint32_t sampler)
	{
		init();

		if (isInit())
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
		}
		else
		{
			LOGE("texture cube[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLCubeArrayTexture::bind()
	{
		init();

		if (isInit())
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_textureId);
			GLHelper::CheckError("fail to bind cube array texture[%s][%u]", m_spec.Name.c_str(), m_textureId);
		}
		else
		{
			LOGE("cube array texture[%s] invalid state[%u]", m_spec.Name.c_str(), m_textureId);
		}
	}

	void GLCubeArrayTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, 0);
	}

	GLBufferTexture::GLBufferTexture(void *data, uint32_t byteSize, ImageInternalFormat format)
		: BufferTexture(), GLTexture()
	{
		m_internalFmt = Image::GetInternalFmt(format);
		m_size = byteSize;
		m_data = data;
	}

	GLBufferTexture::~GLBufferTexture()
	{
		destroy();
	}

	bool GLBufferTexture::init()
	{
		if (!isInit())
		{
			if (!m_data)
			{
				LOGE("BufferTexture: no input data");
				return false;
			}

			// Generate buffer
			glGenBuffers(1, &m_bufferId);
			glBindBuffer(GL_TEXTURE_BUFFER, m_bufferId);
			glBufferData(GL_TEXTURE_BUFFER, m_size, m_data, GL_STATIC_DRAW);

			glGenTextures(1, &m_textureId);
			glBindTexture(GL_TEXTURE_BUFFER, m_textureId);
			// Attach buffer to texture
			glTextureBuffer(m_textureId, GLHelper::GetGLTextureInternalFmt(m_internalFmt), m_bufferId);
			if (!GLHelper::CheckError("fail to attach buffer to texture"))
			{
				glDeleteBuffers(1, &m_bufferId);
				glDeleteTextures(1, &m_textureId);
				m_bufferId = m_textureId = 0;
			}
			glBindTexture(GL_TEXTURE_BUFFER, 0);

			PIO_FREE(m_data);
		}
		return isInit();
	}

	void GLBufferTexture::destroy()
	{
		PIO_FREE(m_data);

		if (isInit())
		{
			glDeleteBuffers(1, &m_bufferId);
			glDeleteTextures(1, &m_textureId);
			m_bufferId = m_textureId = 0;
		}
	}

	void GLBufferTexture::active(uint32_t sampler)
	{
		init();

		if (isInit())
		{
			glActiveTexture(GL_TEXTURE0 + sampler);
		}
		else
		{
			LOGE("buffer texture invalid state[%u][%u]", m_textureId, m_bufferId);
		}
	}

	void GLBufferTexture::bind()
	{
		init();

		if (isInit())
		{
			glBindTexture(GL_TEXTURE_BUFFER, m_textureId);
			GLHelper::CheckError("fail to bind buffer texture[%u][%u]", m_textureId, m_bufferId);
		}
		else
		{
			LOGE("buffer texture invalid state[%u][%u]", m_textureId, m_bufferId);
		}
	}

	void GLBufferTexture::unbind()
	{
		// Only detach the texture from unit, and do not detach buffer 
		glBindTexture(GL_TEXTURE_BUFFER, 0);
	}
}