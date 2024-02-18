#ifndef __PIONNER_PLATFORM_GL_VERTEX_BUFFER_H__
#define __PIONNER_PLATFORM_GL_VERTEX_BUFFER_H__

#include "core/Buffer.h"
#include "gfx/rhi/VertexBuffer.h"

namespace pio
{
	class GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(uint32_t size, BufferUsage usage);
		GLVertexBuffer(const void *data, uint32_t size, BufferUsage usage);
		virtual ~GLVertexBuffer();

		virtual bool init() override;
		virtual void destroy() override;

		virtual void bind() override;
		virtual void unbind() override;

		virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) override;
		virtual void setLayout(const VertexBufferLayout &layout) override { m_layout = layout; }

		virtual bool isInit() const { return m_vbo != 0; };
		virtual const VertexBufferLayout &getLayout() const override { return m_layout; }
		virtual uint32_t getSize() const override { return m_size; }

	private:
		uint32_t m_vbo{ 0 };
		uint32_t m_size{ 0 };
		void *m_data{ nullptr };
		BufferUsage m_usage{ BufferUsage::Static };
		VertexBufferLayout m_layout{};
	};
}

#endif