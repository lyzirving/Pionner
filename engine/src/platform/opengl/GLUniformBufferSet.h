#ifndef __PIONNER_PLATFORM_OPENGL_GL_UNIFORM_BUFFER_SET_H__
#define __PIONNER_PLATFORM_OPENGL_GL_UNIFORM_BUFFER_SET_H__

#include "gfx/rhi/UniformBufferSet.h"

namespace pio
{
	class GLUniformBufferSet : public UniformBufferSet
	{
	public:
		GLUniformBufferSet() {}
		virtual ~GLUniformBufferSet();

		virtual Ref<UniformBuffer> create(uint32_t size, uint32_t binding) override;
		virtual Ref<UniformBuffer> get(uint32_t binding) override { return m_bufferSet[binding]; }

	private:
		std::map<uint32_t, Ref<UniformBuffer>> m_bufferSet{};
	};
}

#endif