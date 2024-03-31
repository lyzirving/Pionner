#include "GLRenderBuffer.h"
#include "GLHelper.h"

#include "gfx/renderer/RenderData.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderBuffer"

namespace pio
{
	GLRenderBuffer::GLRenderBuffer(const TextureSpecification &spec) : RenderBuffer(), m_spec(spec)
	{
		m_spec.Channel = Image::GetChannelCount(m_spec.Format);
		m_internalFmt = Image::GetInternalFmt(m_spec.Format);
		m_pixelType = Image::GetPixelType(m_spec.Format);
		m_size = m_spec.Width * m_spec.Height * m_spec.Channel;
	}

	GLRenderBuffer::~GLRenderBuffer()
	{
		destroy();
	}

	bool GLRenderBuffer::init()
	{
		if (!isInit())
		{
			glGenRenderbuffers(1, &m_bufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetGLTextureInternalFmt(m_internalFmt), 
								  m_spec.Width, m_spec.Height);
			if (GLHelper::CheckError("err happens when init render buffer[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init render buffer[%s][%u]", m_spec.Name.c_str(), m_bufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		return isInit();
	}

	void GLRenderBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_bufferId);
			m_bufferId = 0;
		}
	}

	void GLRenderBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId);
			GLHelper::CheckError("fail to bind render buffer[%s][%u]", m_spec.Name.c_str(), m_bufferId);
		}
		else
		{
			LOGE("RenderBuffer[%s] invalid state[%u]", m_spec.Name.c_str(), m_bufferId);
		}
	}

	void GLRenderBuffer::bind(uint32_t width, uint32_t height)
	{
		init();

		if (isInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_bufferId);
			if (m_spec.Width != width || m_spec.Height != height)
			{
				m_spec.Width = width;
				m_spec.Height = height;
				glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetGLTextureInternalFmt(m_internalFmt),
									  m_spec.Width, m_spec.Height);
			}
			GLHelper::CheckError("fail to bind render buffer[%s][%u]", m_spec.Name.c_str(), m_bufferId);
		}
		else
		{
			LOGE("RenderBuffer[%s] invalid state[%u]", m_spec.Name.c_str(), m_bufferId);
		}
	}

	void GLRenderBuffer::unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}