#ifndef __GL_VERTEX_BUFFER_H__
#define __GL_VERTEX_BUFFER_H__

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

		virtual void bind() override;
		virtual void bindToTarget(uint32_t target = 0) override;
		virtual uint32_t size() override { return m_vertex.size(); }

	private:
		std::vector<Vertex> m_vertex;
	};

	template<>
	bool GfxBuffer::is<GLVertexBuffer>() const;

	template<>
	GLVertexBuffer *GfxBuffer::getPtr<GLVertexBuffer>();

	template<>
	void GfxBuffer::insertData<Vertex>(std::vector<Vertex> &data);
}

#endif