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

		virtual void setData(uint32_t indiceNum, const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual uint32_t indiceNum() const = 0;

	public:
		static Ref<IndexBuffer> Create(Ref<RenderContext>& context, uint32_t size, uint32_t indiceNum, BufferUsage usage = BufferUsage::Static);
		static Ref<IndexBuffer> Create(Ref<RenderContext>& context, const void *data, uint32_t size, uint32_t indiceNum, BufferUsage usage = BufferUsage::Static);
	};
}

#endif