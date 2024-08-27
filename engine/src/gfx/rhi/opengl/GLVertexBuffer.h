#ifndef __PIONNER_GFX_RHI_OPENGL_GL_VERTEX_BUFFER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_VERTEX_BUFFER_H__

#include "gfx/rhi/VertexBuffer.h"

namespace pio
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(Ref<RenderContext>& context, uint32_t size, BufferUsage usage);
		GLVertexBuffer(Ref<RenderContext>& context, const void *data, uint32_t size, BufferUsage usage);
		virtual ~GLVertexBuffer() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return m_size; }
		virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) override;

		virtual void setBufferLayout(const VertexBufferLayout &layout) override { m_layout = layout; }
		virtual const VertexBufferLayout &layout() const override { return m_layout; }

	private:
		BufferUsage m_usage{ BufferUsage::Static };
		VertexBufferLayout m_layout{};
	};
}

#endif