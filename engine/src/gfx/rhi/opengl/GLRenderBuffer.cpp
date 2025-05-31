#include "GLRenderBuffer.h"

#include "GLHeader.h"
#include "GLHelper.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLRenderBuffer"

namespace pio
{
	GLRenderBuffer::GLRenderBuffer(const Ref<RenderContext>& context, const TextureSpecific& spec)
		: RenderBuffer(context, spec)
	{
	}

	bool GLRenderBuffer::Init()
	{
		if (!IsInit())
		{
			glGenRenderbuffers(1, &m_ID);
			glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
			glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetTextureInternalFmt(m_Spec.Format), m_Spec.Width, m_Spec.Height);
			if (GLHelper::CheckError("err happens when init render buffer[%s]", m_Spec.Name.c_str()))
				LOGD("succeed to init render buffer[%s][%u]", m_Spec.Name.c_str(), m_ID);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}
		return IsInit();
	}

	void GLRenderBuffer::Destroy()
	{
		if (IsInit())
		{
			glDeleteBuffers(1, &m_ID);
			m_ID = 0;
		}
	}

	void GLRenderBuffer::Bind()
	{
		Init();

		if (IsInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
			m_Bound = GLHelper::CheckError("err! fail to bind render buffer[%s][%u]", m_Spec.Name.c_str(), m_ID);
		}
		else
		{
			LOGE("err! invalid state[%s]", m_Spec.Name.c_str());
		}
	}

	void GLRenderBuffer::Bind(uint32_t width, uint32_t height)
	{
		Init();

		if (IsInit())
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_ID);
			if (m_Spec.Width != width || m_Spec.Height != height)
			{
				m_Spec.Width = width;
				m_Spec.Height = height;
				m_Size = m_Spec.Width * m_Spec.Height * m_Spec.Channel * GetTextureByteSize(m_Spec.Format);
				glRenderbufferStorage(GL_RENDERBUFFER, GLHelper::GetTextureInternalFmt(m_Spec.Format), m_Spec.Width, m_Spec.Height);
			}
			m_Bound = GLHelper::CheckError("err! fail to bind render buffer[%s][%u]", m_Spec.Name.c_str(), m_ID);
		}
		else
		{
			LOGE("err! invalid state[%s]", m_Spec.Name.c_str());
		}
	}

	void GLRenderBuffer::UnBind()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		m_Bound = false;
	}
}