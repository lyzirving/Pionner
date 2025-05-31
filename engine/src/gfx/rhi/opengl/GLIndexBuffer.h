#pragma once

#include "gfx/rhi/IndexBuffer.h"

namespace pio
{
	class GLIndexBuffer : public IndexBuffer
	{
		RTTR_ENABLE(IndexBuffer)
	public:
		GLIndexBuffer(const Ref<RenderContext>& context, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt, BufferUsage usage);
		GLIndexBuffer(const Ref<RenderContext>& context, const void *data, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt, BufferUsage usage);
		virtual ~GLIndexBuffer() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const { return m_ID != 0; }

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual uint32_t Size() const override { return m_Size; }

		virtual void SetIndice(const void* data, uint32_t size, uint32_t indiceNum, uint32_t offset = 0) override;		
		virtual uint32_t IndexCount() const override { return m_IndexCount; }
		virtual IndexInternalFmt InternalFmt() const override { return m_Format; }

	private:
		BufferUsage m_Usage{ BufferUsage::Static };
		IndexInternalFmt m_Format{ IndexInternalFmt::U_BYTE };
		uint32_t m_IndexCount{ 0 };
	};
}