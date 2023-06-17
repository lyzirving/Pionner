#include "function/render/rhi/opengl/buffer/GLIndexBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

#include "core/log/LogSystem.h"	

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLIndexBuffer"

namespace Pionner
{
	GLIndexBuffer::GLIndexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr)
	{
		m_bufferType = BUF_INDICE;
	}

	GLIndexBuffer::~GLIndexBuffer() = default;

	void GLIndexBuffer::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

		if (isUpload())
			return;

		if (m_indices.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
			LOG_DEBUG("create indice buffer[%u]", m_id);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32_t), &m_indices[0], GL_STATIC_DRAW);

		m_uploaded = GLHelper::checkGLErr("fail to build indices buffer");
	}

	void GLIndexBuffer::bind()
	{
		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

		if (!isUpload())
		{
			upload();
		}

		if (isUpload())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		}
	}

	void GLIndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GLIndexBuffer::deleteResource()
	{
		if (isCreated())
		{
			//LOG_DEBUG("buf id[%u]", m_id);
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	template<>
	bool GfxBuffer::is<GLIndexBuffer>() const
	{
		return getBufferType() == BUF_INDICE;
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