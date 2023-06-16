#include "GLDepthFrameBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "core/log/LogSystem.h"	

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLDepthFrameBuffer"

namespace Pionner
{
	GLDepthFrameBuffer::GLDepthFrameBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxFrameBuffer(mgr)
	{
	}

	GLDepthFrameBuffer::GLDepthFrameBuffer(uint32_t width, uint32_t height, const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxFrameBuffer(width, height, mgr)
	{
	}

	GLDepthFrameBuffer::~GLDepthFrameBuffer()
	{
	}

	void GLDepthFrameBuffer::upload()
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
		}

		m_uploaded = true;
	}

	void GLDepthFrameBuffer::bind()
	{
	}

	void GLDepthFrameBuffer::unbind()
	{
	}

	void GLDepthFrameBuffer::deleteResource()
	{
	}
}