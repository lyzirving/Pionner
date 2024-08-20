#include "UniformBuffer.h"

#include "base/Math.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBuffer"

namespace pio
{
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding, BufferUsage usage)
	{
		return Ref<UniformBuffer>();
	}
}