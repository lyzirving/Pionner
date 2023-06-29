#include "GLCubeDepthBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"	

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLCubeDepthBuffer"

namespace Pionner
{
	GLCubeDepthBuffer::GLCubeDepthBuffer(const std::shared_ptr<RenderResourceMgr> &mgr) 
		: GLDepthBuffer(mgr)
	{
		m_bufferType = BUF_CUBE_DEPTH_FBO;
	}

	GLCubeDepthBuffer::GLCubeDepthBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GLDepthBuffer(width, height, mgr)
	{
		m_bufferType = BUF_CUBE_DEPTH_FBO;
	}

	GLCubeDepthBuffer::~GLCubeDepthBuffer() = default;

	void GLCubeDepthBuffer::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("cube depth fbo is already abandoned");
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

			glBindTexture(GL_TEXTURE_CUBE_MAP, m_attachIds[DEPTH_ATTACH]);
			for (uint32_t i = 0; i < 6; ++i)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
							 m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			
			// attach depth texture as FBO's depth buffer
			glBindFramebuffer(GL_FRAMEBUFFER, m_attachIds[DEPTH_ATTACH]);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_attachIds[DEPTH_ATTACH], 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			uint32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (status == GL_FRAMEBUFFER_COMPLETE)
			{
				LOG_DEBUG("succeed to create cube shadow map, size[%u, %u], fbo[%u], texture id[%u]",
						  m_width, m_height, m_id, m_attachIds[DEPTH_ATTACH]);
				m_uploaded = true;
			}
			else
			{
				LOG_ERR("fail to create cube shadow map, status[%x]", status);
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	template<>
	bool GfxBuffer::is<GLCubeDepthBuffer>() const
	{
		return getBufferType() == BUF_CUBE_DEPTH_FBO;
	}

	template<>
	GLCubeDepthBuffer *GfxBuffer::getPtr<GLCubeDepthBuffer>()
	{
		if (is<GLCubeDepthBuffer>())
		{
			GLCubeDepthBuffer *ret = dynamic_cast<GLCubeDepthBuffer *>(this);
			return ret;
		}
		return nullptr;
	}

}