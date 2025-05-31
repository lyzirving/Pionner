#include "UniformBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLUniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBuffer"

namespace pio
{
	Ref<UniformBuffer> UniformBuffer::Create(const Ref<RenderContext>& context, uint32_t size, UBBinding binding, BufferUsage usage)
	{
		switch(context->RenderBackend())
		{
			case RenderBackend_OpenGL:
				return CreateRef<GLUniformBuffer>(context, size, binding, usage);
			case RenderBackend_Vulkan:
			default:
				LOGE("Err! backend[%u] has not been implemented", context->RenderBackend());
				std::abort();
				return Ref<UniformBuffer>();
		}
	}
}