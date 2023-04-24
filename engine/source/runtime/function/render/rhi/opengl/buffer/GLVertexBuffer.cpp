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
		if (m_vertex.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
		}

		if (!isLoad())
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(Vertex), &m_vertex[0], GL_STATIC_DRAW);
			m_loaded = true;
		}
	}

	void GLVertexBuffer::release()
	{
		if (isLoad())
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
}