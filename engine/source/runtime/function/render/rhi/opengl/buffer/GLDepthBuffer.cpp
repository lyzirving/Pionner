#include "GLDepthBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"	

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLDepthBuffer"

namespace Pionner
{
	GLDepthBuffer::GLDepthBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxFrameBuffer(mgr)
	{
		m_bufferType = BUF_DEPTH_FBO;
	}

	GLDepthBuffer::GLDepthBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxFrameBuffer(width, height, mgr)
	{
		m_bufferType = BUF_DEPTH_FBO;
	}

	GLDepthBuffer::~GLDepthBuffer() = default;

	void GLDepthBuffer::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("depth fbo is already abandoned");
			return;
		}

		if (isUpload())
			return;

		if (m_width == 0 || m_height == 0)
		{
			LOG_ERR("invalid size[%u, %u]", m_width, m_height);
			return;
		}

		if (!isCreated())
		{
			glGenFramebuffers(1, &m_id);
			glGenTextures(1, &m_attachIds[DEPTH_ATTACH]);

			glBindTexture(GL_TEXTURE_2D, m_attachIds[DEPTH_ATTACH]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_attachIds[DEPTH_ATTACH], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status == GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_DEBUG("succeed to create shadow map, size[%u, %u], fbo[%u], texture id[%u]",
						  m_width, m_height, m_id, m_attachIds[DEPTH_ATTACH]);
				m_uploaded = true;
			}
			else
			{
				LOG_ERR("fail to create shadow map, status[%x]", status);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void GLDepthBuffer::bind()
	{
		if (isAbandonded())
		{
			LOG_ERR("depth fbo is already abandoned");
			return;
		}

		if (!isUpload())
		{
			upload();
		}

		if (isUpload())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			GLHelper::checkGLErr("fail to bind depth framebuffer");
		}
	}

	void GLDepthBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void GLDepthBuffer::deleteResource()
	{
		if (m_attachIds[DEPTH_ATTACH] != 0)
		{
			glDeleteTextures(1, &m_attachIds[DEPTH_ATTACH]);
			m_attachIds[DEPTH_ATTACH] = 0;
		}

		if (m_id != 0)
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}
	}

	template<>
	bool GfxBuffer::is<GLDepthBuffer>() const
	{
		return getBufferType() == BUF_DEPTH_FBO;
	}

	template<>
	GLDepthBuffer *GfxBuffer::getPtr<GLDepthBuffer>()
	{
		if (is<GLDepthBuffer>())
		{
			GLDepthBuffer *ret = dynamic_cast<GLDepthBuffer *>(this);
			return ret;
		}
		return nullptr;
	}
}