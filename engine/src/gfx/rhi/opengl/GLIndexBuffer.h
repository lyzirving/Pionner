#ifndef __PIONNER_GFX_RHI_OPENGL_GL_INDEX_BUFFER_H__
#define __PIONNER_GFX_RHI_OPENGL_GL_INDEX_BUFFER_H__

#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class GLIndexBuffer : public IndexBuffer
	{
	public:
		GLIndexBuffer(Ref<RenderContext>& context, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage);
		GLIndexBuffer(Ref<RenderContext>& context, const void *data, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage);
		virtual ~GLIndexBuffer() = default;

		virtual bool init() override;
		virtual void destroy() override;
		virtual bool isInit() const { return m_id != 0; }

		virtual void bind() override;
		virtual void unbind() override;

		virtual uint32_t size() const override { return m_size; }

		virtual void setIndice(const void* data, uint32_t size, uint32_t indiceNum, uint32_t offset = 0) override;		
		virtual uint32_t indexCount() const override { return m_indexCount; }
		virtual IndexInternalFmt internalFmt() const override { return m_format; }

	private:
		BufferUsage m_usage{ BufferUsage::Static };
		IndexInternalFmt m_format{ IndexInternalFmt::U_BYTE };
		uint32_t m_indexCount{ 0 };
	};
}

#endif