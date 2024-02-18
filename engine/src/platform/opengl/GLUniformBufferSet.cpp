#include "GLUniformBufferSet.h"

#include "GLUniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GLUniformBufferSet"

namespace pio
{
	GLUniformBufferSet::~GLUniformBufferSet()
	{
		auto it = m_bufferSet.begin();
		while (it != m_bufferSet.end())
		{
			it->second.reset();
			it = m_bufferSet.erase(it);
		}
	}

	Ref<UniformBuffer> GLUniformBufferSet::create(uint32_t size, uint32_t binding)
	{
		m_bufferSet[binding] = CreateRef<UniformBuffer, GLUniformBuffer>(size, binding);
		return m_bufferSet[binding];
	}
}