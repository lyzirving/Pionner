#ifndef __PIONNER_GFX_RHI_INDEX_BUFFER_H__
#define __PIONNER_GFX_RHI_INDEX_BUFFER_H__

#include "RenderResource.h"

namespace pio
{
	class IndexBuffer : public RenderResource
	{
	public:
		IndexBuffer(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::EBO) {}
		virtual ~IndexBuffer() = default;
		
		virtual uint32_t indexCount() const = 0;
		virtual IndexInternalFmt internalFmt() const = 0;

	public:
		static Ref<IndexBuffer> Create(Ref<RenderContext>& context, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt = IndexInternalFmt::U_BYTE, BufferUsage usage = BufferUsage::Static);
		static Ref<IndexBuffer> Create(Ref<RenderContext>& context, const void *data, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt = IndexInternalFmt::U_BYTE, BufferUsage usage = BufferUsage::Static);
	};

	namespace Rhi
	{
		template<>
		IndexInternalFmt GetIndexInternalFmt<uint8_t>();

		template<>
		IndexInternalFmt GetIndexInternalFmt<uint16_t>();

		template<>
		IndexInternalFmt GetIndexInternalFmt<uint32_t>();
	}
}

#endif