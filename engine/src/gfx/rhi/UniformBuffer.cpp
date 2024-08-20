#include "UniformBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLUniformBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "UniformBuffer"

namespace pio
{
	Ref<UniformBuffer> UniformBuffer::Create(Ref<RenderContext>& context, uint32_t size, uint32_t binding, BufferUsage usage)
	{
		switch (context->backendFlag())
		{
		case Backend_OpenGL:
			return CreateRef<GLUniformBuffer>(context, size, binding, usage);
		case Backend_Vulkan:
		default:
			LOGE("Err! backend[%u] has not been implemented", context->backendFlag());
			std::abort();
			return Ref<UniformBuffer>();
		}
	}
}