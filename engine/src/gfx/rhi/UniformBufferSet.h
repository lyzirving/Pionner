#ifndef __PIONNER_GFX_RHI_UNIFORM_BUFFER_SET_H__
#define __PIONNER_GFX_RHI_UNIFORM_BUFFER_SET_H__

#include "UniformBuffer.h"

namespace pio
{
	class UniformBufferSet
	{
	public:
		virtual ~UniformBufferSet() {}

		virtual Ref<UniformBuffer> create(uint32_t size, uint32_t binding) = 0;
		virtual Ref<UniformBuffer> get(uint32_t binding) = 0;

		static Ref<UniformBufferSet> Create();
	};
}

#endif