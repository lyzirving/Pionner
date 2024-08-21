#ifndef __PIONNER_GFX_RHI_UNIFORM_BUFFER_H__
#define __PIONNER_GFX_RHI_UNIFORM_BUFFER_H__

#include "Uniform.h"
#include "RenderResource.h"

namespace pio
{	
	class RenderContext;

	class UniformBuffer : public RenderResource
	{
	public:
		UniformBuffer(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::UBO) {}
		virtual ~UniformBuffer() = default;

		virtual void bind() = 0;
		virtual void bind(int64_t offset, int64_t size) = 0;
		virtual void unbind() = 0;

		virtual void setData(const void *data, uint32_t size, uint32_t offset = 0) = 0;
	
		virtual uint32_t getBinding() const = 0;
		virtual uint32_t getSize() const = 0;

	public:
		static Ref<UniformBuffer> Create(Ref<RenderContext> &context, uint32_t size, uint32_t binding, BufferUsage usage = BufferUsage::DynamicRead);
	};
}

#endif