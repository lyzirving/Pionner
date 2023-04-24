#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "function/render/resource/buffer/GfxBuffer.h"

namespace Pionner
{
	struct Vertex;

	class GLVertexBuffer : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLVertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLVertexBuffer();

		virtual void upload() override;
		virtual void release() override;

		virtual void bindToTarget(uint32_t target = 0) override;

	private:
		std::vector<Vertex> m_vertex;
	};

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

#endif