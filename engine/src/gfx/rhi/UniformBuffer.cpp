#include "UniformBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLUniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBuffer"

namespace pio
{
	Ref<UniformBuffer> UniformBuffer::Create(Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage)
	{
		switch(context->renderBackend())
		{
			case RenderBackend_OpenGL:
				return CreateRef<GLUniformBuffer>(context, size, binding, usage);
			case RenderBackend_Vulkan:
			default:
				LOGE("Err! backend[%u] has not been implemented", context->renderBackend());
				std::abort();
				return Ref<UniformBuffer>();
		}
	}

	UniformBufferSet::~UniformBufferSet()
	{
		release();
	}

	UniformBufferSet::UniformBufferSet(const UniformBufferSet& rhs)
	{
		m_bufferSet = rhs.m_bufferSet;
	}

	UniformBufferSet::UniformBufferSet(UniformBufferSet&& rhs) noexcept
	{
		m_bufferSet = std::move(rhs.m_bufferSet);
	}

	UniformBufferSet& UniformBufferSet::operator=(const UniformBufferSet& rhs)
	{
		if(this != &rhs)
		{
			this->UniformBufferSet::UniformBufferSet(rhs);
		}
		return *this;
	}

	UniformBufferSet& UniformBufferSet::operator=(UniformBufferSet&& rhs) noexcept
	{
		if(this != &rhs)
		{
			this->UniformBufferSet::UniformBufferSet(std::forward<UniformBufferSet>(rhs));
		}
		return *this;
	}

	void UniformBufferSet::release()
	{
		auto it = m_bufferSet.begin();
		while(it != m_bufferSet.end())
		{
			it->second.reset();
			it = m_bufferSet.erase(it);
		}
	}

	void UniformBufferSet::put(Ref<UniformBuffer>& unimBuff)
	{
		if(!unimBuff)
			return;

		if(m_bufferSet.find(unimBuff->binding()) == m_bufferSet.end())
		{
			m_bufferSet[unimBuff->binding()] = unimBuff;
		}
	}

	Ref<UniformBuffer>& UniformBufferSet::get(uint32_t binding)
	{
		if(m_bufferSet.find(binding) != m_bufferSet.end())
		{
			return m_bufferSet[binding];
		}
		return Ref<UniformBuffer>();
	}
}