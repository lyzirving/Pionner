#ifndef __GL_VERTEX_BUFFER_H__
#define __GL_VERTEX_BUFFER_H__

#include "render/resource/buffer/GfxBuffer.h"

namespace pio
{
	struct Vertex;

	class GLVertexBuffer : public GfxBuffer
	{
		friend class GfxBuffer;
	public:
		GLVertexBuffer(const std::shared_ptr<RenderResourceMgr> &mgr);
		virtual ~GLVertexBuffer();

		virtual void upload() override;

		virtual void bind() override;
		virtual void unbind() override;
		virtual void deleteResource() override;
		virtual uint32_t size() override { return m_vertex.size(); }

	private:
		std::vector<Vertex> m_vertex;
		uint32_t m_vao;
	};

	template<>
	bool GfxBuffer::is<GLVertexBuffer>() const;

	template<>
	GLVertexBuffer *GfxBuffer::getPtr<GLVertexBuffer>();

	template<>
	void GfxBuffer::insertData<Vertex>(std::vector<Vertex> &data);
}

#endif