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
		if (isUpload())
			return;

		if (m_indices.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);
		m_uploaded = true;
	}

	void GLIndexBuffer::release()
	{
		if (isCreated())
		{
			std::shared_ptr<RenderResourceMgr> mgr = m_mgr.lock();
			if (mgr)
			{
				mgr->release(m_bufferType, m_slot);
			}
		}
	}

	void GLIndexBuffer::bind()
	{
		if (!isUpload())
		{
			upload();
		}

		if (isUpload())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}
	}

	template<>
	bool GfxBuffer::is<GLIndexBuffer>() const
	{
		return getDataType() == DATA_INDICE;
	}

	template<>
	void GfxBuffer::insertData<uint32_t>(std::vector<uint32_t> &data)
	{
		if (is<GLIndexBuffer>())
		{
			GLIndexBuffer *ptr = static_cast<GLIndexBuffer *>(this);
			ptr->m_indices.assign(data.begin(), data.end());
		}
	}
}