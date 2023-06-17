#include "GLVertexBuffer.h"

#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

#include "core/log/LogSystem.h"	

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLVertexBuffer"

namespace Pionner
{
	GLVertexBuffer::GLVertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_vertex(), m_vao(0)
	{
		m_bufferType = BUF_VERTEX;
	}

	GLVertexBuffer::~GLVertexBuffer() = default;

	void GLVertexBuffer::upload()
	{
		if (isAbandonded())
		{
			LOG_ERR("buf is already abandoned");
			return;
		}

		if (isUpload())
			return;

		if (m_vertex.empty())
			return;

		if (!isCreated())
		{
			glGenVertexArrays(1, &m_vao);
			glGenBuffers(1, &m_id);
			LOG_DEBUG("create vertex buffer[%u], vao[%u]", m_id, m_vao);
		}

		// step 1: upload data 
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(Vertex), &m_vertex[0], GL_STATIC_DRAW);

		// step 2: bind data to vao 
		glBindVertexArray(m_vao);

		// bind buffer to vao
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		// set vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);
		// set vertex normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
		// set texture coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

		// step 3: unbind vao
		glBindVertexArray(0);

		m_uploaded = GLHelper::checkGLErr("fail to build vertex buffer");
	}

	void GLVertexBuffer::bind()
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
			glBindVertexArray(m_vao);
		}
	}

	void GLVertexBuffer::unbind()
	{
		glBindVertexArray(0);
	}

	void GLVertexBuffer::deleteResource()
	{
		if (isCreated())
		{
			//LOG_DEBUG("buf id[%u], vao[%u]", m_id, m_vao);
			glDeleteBuffers(1, &m_id);
			glDeleteVertexArrays(1, &m_vao);
			m_id = 0;
			m_vao = 0;
		}
	}

	template<>
	bool GfxBuffer::is<GLVertexBuffer>() const
	{
		return getBufferType() == BUF_VERTEX;
	}

	template<>
	GLVertexBuffer *GfxBuffer::getPtr<GLVertexBuffer>()
	{
		if (is<GLVertexBuffer>())
		{
			GLVertexBuffer *ret = dynamic_cast<GLVertexBuffer *>(this);
			return ret;
		}
		return nullptr;
	}

	template<>
	void GfxBuffer::insertData<Vertex>(std::vector<Vertex> &data)
	{
		if (is<GLVertexBuffer>())
		{
			GLVertexBuffer *ptr = static_cast<GLVertexBuffer *>(this);
			ptr->m_vertex.assign(data.begin(), data.end());
		}
	}
}