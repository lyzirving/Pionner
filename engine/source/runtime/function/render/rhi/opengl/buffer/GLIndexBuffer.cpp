#include "function/render/rhi/opengl/buffer/GLIndexBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	GLIndexBuffer::GLIndexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr)
	{
		m_bufferType = BUF_EBO;
		m_dataType = DATA_INDICE;
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		GLIndexBuffer::release();
	}

	void GLIndexBuffer::upload()
	{
		if (m_indices.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
		}

		if (!isLoad())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);
			m_loaded = true;
		}
	}

	void GLIndexBuffer::release()
	{
	}

	void GLIndexBuffer::bind()
	{
		if (!isLoad())
		{
			upload();
		}

		if (isLoad())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}
	}
}