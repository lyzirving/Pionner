#pragma once

#include "gfx/rhi/VertexBuffer.h"

namespace pio
{
	class GLVertexBuffer : public VertexBuffer
	{
		RTTR_ENABLE(VertexBuffer)
	public:
		GLVertexBuffer(const Ref<RenderContext>& context, uint32_t size, BufferUsage usage);
		GLVertexBuffer(const Ref<RenderContext>& context, const void *data, uint32_t size, BufferUsage usage);
		virtual ~GLVertexBuffer() = default;

		virtual bool Init() override;
		virtual void Destroy() override;
		virtual bool IsInit() const { return m_ID != 0; }

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual uint32_t Size() const override { return m_Size; }
		virtual void SetData(const void *data, uint32_t size, uint32_t offset = 0) override;

		virtual void SetBufferLayout(const VertexBufferLayout &layout) override { m_Layout = layout; }
		virtual const VertexBufferLayout &Layout() const override { return m_Layout; }

	private:
		BufferUsage m_Usage{ BufferUsage::Static };
		VertexBufferLayout m_Layout{};
	};
}