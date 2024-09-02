#ifndef __PIONNER_GFX_RHI_UNIFORM_BUFFER_H__
#define __PIONNER_GFX_RHI_UNIFORM_BUFFER_H__

#include "UniformPack.h"
#include "RenderResource.h"

namespace pio
{	
	class RenderContext;

	class UniformBuffer : public RenderResource
	{
	public:
		UniformBuffer(Ref<RenderContext>& context) : RenderResource(context, RenderResourceType::UBO) {}
		virtual ~UniformBuffer() = default;

		virtual void bindAt(int64_t offset, int64_t size) = 0;
		virtual UBBinding binding() const = 0;

	public:
		static Ref<UniformBuffer> Create(Ref<RenderContext> &context, uint32_t size, UBBinding binding, BufferUsage usage = BufferUsage::DynamicRead);
	};

	class UniformBufferSet
	{
	public:
		UniformBufferSet() {}
		~UniformBufferSet();

		UniformBufferSet(const UniformBufferSet& rhs);
		UniformBufferSet(UniformBufferSet&& rhs) noexcept;

		UniformBufferSet& operator=(const UniformBufferSet& rhs);
		UniformBufferSet& operator=(UniformBufferSet&& rhs) noexcept;

		void release();

		void put(Ref<UniformBuffer>& unimBuff);
		Ref<UniformBuffer>& get(uint32_t binding);
		Ref<UniformBuffer>& operator[](uint32_t binding) { return get(binding); }

	private:
		std::map<uint32_t, Ref<UniformBuffer>> m_bufferSet{};
	};
}

#endif