#include "function/render/rhi/opengl/buffer/GLVertexBuffer.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	GLVertexBuffer::GLVertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_vertex()
	{
		m_bufferType = BUF_MEM_ARRAY;
		m_dataType = DATA_VERTEX;
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		GLVertexBuffer::release();
	}

	void GLVertexBuffer::upload()
	{
		if (isLoad())
			return;

		if (m_vertex.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(Vertex), &m_vertex[0], GL_STATIC_DRAW);
		m_loaded = true;
	}

	void GLVertexBuffer::release()
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

	void GLVertexBuffer::bindToTarget(uint32_t target)
	{
		GLVertexBuffer::upload();

		glEnableVertexAttribArray(target);
		glVertexAttribPointer(target, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)nullptr);

		glEnableVertexAttribArray(target + 1);
		glVertexAttribPointer(target + 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

		glEnableVertexAttribArray(target + 2);
		glVertexAttribPointer(target + 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
	}

	template<>
	bool GfxBuffer::is<GLVertexBuffer>() const
	{
		return getDataType() == DATA_VERTEX;
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