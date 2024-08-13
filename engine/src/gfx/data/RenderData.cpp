#include "RenderData.h"

namespace pio
{
	void RenderData::create(uint32_t size, uint32_t binding, BufferUsage usage)
	{
		m_buffer = UniformBuffer::Create(size, binding, usage);
	}
}