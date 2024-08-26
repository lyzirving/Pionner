#include "IndexBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLIndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndexBuffer"

namespace pio
{
	Ref<IndexBuffer> IndexBuffer::Create(Ref<RenderContext>& context, uint32_t size, uint32_t indiceNum, BufferUsage usage)
	{
		switch (context->backendFlag())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLIndexBuffer>(context, size, indiceNum, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->backendFlag());
			std::abort();
			return Ref<IndexBuffer>();
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(Ref<RenderContext>& context, const void* data, uint32_t size, uint32_t indiceNum, BufferUsage usage)
	{
		switch (context->backendFlag())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLIndexBuffer>(context, data, size, indiceNum, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->backendFlag());
			std::abort();
			return Ref<IndexBuffer>();
		}
	}
}