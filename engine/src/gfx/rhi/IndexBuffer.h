#pragma once

#include "RenderResource.h"

namespace pio
{
	class IndexBuffer : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		IndexBuffer(const Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::EBO) {}
		virtual ~IndexBuffer() = default;
		
		virtual uint32_t IndexCount() const = 0;
		virtual IndexInternalFmt InternalFmt() const = 0;

	public:
		static Ref<IndexBuffer> Create(const Ref<RenderContext>& context, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt = IndexInternalFmt::U_BYTE, BufferUsage usage = BufferUsage::Static);
		static Ref<IndexBuffer> Create(const Ref<RenderContext>& context, const void *data, uint32_t size, uint32_t IndexCount, IndexInternalFmt InternalFmt = IndexInternalFmt::U_BYTE, BufferUsage usage = BufferUsage::Static);
	};

	template<>
	IndexInternalFmt GetIndexInternalFmt<uint8_t>();

	template<>
	IndexInternalFmt GetIndexInternalFmt<uint16_t>();

	template<>
	IndexInternalFmt GetIndexInternalFmt<uint32_t>();
}