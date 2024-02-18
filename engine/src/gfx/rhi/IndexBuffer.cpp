#include "IndexBuffer.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLIndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndexBuffer"

namespace pio
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t size, uint32_t num)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<IndexBuffer, GLIndexBuffer>(size, num);
			default:
				return Ref<IndexBuffer>();
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(const void *data, uint32_t size, uint32_t num)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<IndexBuffer, GLIndexBuffer>(data, size, num);
			default:
				return Ref<IndexBuffer>();
		}
	}
}