#pragma once

#include "UniformStorageLayout.h"
#include "UniformBlock.h"
#include "RenderResource.h"

namespace pio
{	
	class Shader;
	class RenderContext;

	class UniformBuffer : public RenderResource
	{
		RTTR_ENABLE(RenderResource)
	public:
		UniformBuffer(const Ref<RenderContext>& context, const std::string& name) : RenderResource(context, RenderResourceType::UBO, name) {}		
		virtual ~UniformBuffer() = default;

		virtual bool BindBlock(const Ref<RenderContext>& context, const Ref<Shader>& shader) = 0;
		virtual void BindAt(int64_t offset, int64_t size) = 0;
		virtual UBBinding Binding() const = 0;

	protected:
		UniformBuffer(const Ref<RenderContext>& context, const std::string& name, RenderResourceType type) : RenderResource(context, type, name) {}

	public:
		static Ref<UniformBuffer> Create(const Ref<RenderContext> &context, uint32_t size, UBBinding binding, BufferUsage usage = BufferUsage::DynamicRead);
	};
}