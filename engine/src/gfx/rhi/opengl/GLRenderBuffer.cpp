#include "GLRenderBuffer.h"

#include "GLHeader.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderBuffer"

namespace pio
{
	GLRenderBuffer::GLRenderBuffer(Ref<RenderContext>& context, const TextureSpecific& spec)
		: RenderBuffer(context, spec)
	{
	}

	bool GLRenderBuffer::init()
	{
		if (!isInit())
		{
			glGenRenderbuffers(1, &m_id);
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
			glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetTextureInternalFmt(m_spec.Format), m_spec.Width, m_spec.Height);
			if (GLHelper::CheckError("err happens when init render buffer[%s]", m_spec.Name.c_str()))
				LOGD("succeed to init render buffer[%s][%u]", m_spec.Name.c_str(), m_id);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		return isInit();
	}

	void GLRenderBuffer::destroy()
	{
		if (isInit())
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	void GLRenderBuffer::bind()
	{
		init();

		if (isInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
			GLHelper::CheckError("err! fail to bind render buffer[%s][%u]", m_spec.Name.c_str(), m_id);
		}
		else
		{
			LOGE("err! invalid state[%s]", m_spec.Name.c_str());
		}
	}

	void GLRenderBuffer::bind(uint32_t width, uint32_t height)
	{
		init();

		if (isInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
			if (m_spec.Width != width || m_spec.Height != height)
			{
				m_spec.Width = width;
				m_spec.Height = height;
				m_size = m_spec.Width * m_spec.Height * m_spec.Channel * Rhi::GetTextureByteSize(m_spec.Format);
				glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetTextureInternalFmt(m_spec.Format), m_spec.Width, m_spec.Height);
			}
			GLHelper::CheckError("err! fail to bind render buffer[%s][%u]", m_spec.Name.c_str(), m_id);
		}
		else
		{
			LOGE("err! invalid state[%s]", m_spec.Name.c_str());
		}
	}

	void GLRenderBuffer::unbind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}