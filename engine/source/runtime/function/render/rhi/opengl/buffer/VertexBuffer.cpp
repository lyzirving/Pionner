#include "function/render/rhi/opengl/buffer/VertexBuffer.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/rhi/RhiHeader.h"

#include "function/render/resource/RenderResourceMgr.h"

namespace Pionner
{
	VertexBuffer::VertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr)
		: GfxBuffer(mgr), m_vertex()
	{
		m_bufferType = BUF_MEM_ARRAY;
		m_dataType = DATA_VERTEX;
	}

	VertexBuffer::~VertexBuffer()
	{
		VertexBuffer::release();
	}

	void VertexBuffer::upload()
	{
		if (m_vertex.empty())
			return;

		if (!isCreated())
		{
			glGenBuffers(1, &m_id);
		}

		if (!m_loaded)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
			glBufferData(GL_ARRAY_BUFFER, m_vertex.size() * sizeof(GfxVertex), &m_vertex[0], GL_STATIC_DRAW);
			m_loaded = true;
		}
	}

	void VertexBuffer::release()
	{
		if (m_loaded)
		{
			std::shared_ptr<RenderResourceMgr> mgr = m_mgr.lock();
			if (mgr)
			{
				mgr->release(m_bufferType, m_id);
			}
		}
	}

	void VertexBuffer::bindToTarget(uint32_t target)
	{
		VertexBuffer::upload();

		glEnableVertexAttribArray(target);
		glVertexAttribPointer(target, 3, GL_FLOAT, GL_FALSE, sizeof(GfxVertex), (void *)nullptr);

		glEnableVertexAttribArray(target + 1);
		glVertexAttribPointer(target + 1, 3, GL_FLOAT, GL_FALSE, sizeof(GfxVertex), (void *)offsetof(GfxVertex, normal));

		glEnableVertexAttribArray(target + 2);
		glVertexAttribPointer(target + 2, 2, GL_FLOAT, GL_FALSE, sizeof(GfxVertex), (void *)offsetof(GfxVertex, texCoord));
	}
}