#include "IndexBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLIndexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "IndexBuffer"

namespace pio
{
	Ref<IndexBuffer> IndexBuffer::Create(Ref<RenderContext>& context, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLIndexBuffer>(context, size, indexCount, internalFmt, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->renderBackend());
			std::abort();
			return Ref<IndexBuffer>();
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(Ref<RenderContext>& context, const void* data, uint32_t size, uint32_t indexCount, IndexInternalFmt internalFmt, BufferUsage usage)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLIndexBuffer>(context, data, size, indexCount, internalFmt, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->renderBackend());
			std::abort();
			return Ref<IndexBuffer>();
		}
	}

	namespace Rhi
	{
		template<>
		IndexInternalFmt GetIndexInternalFmt<uint8_t>() { return IndexInternalFmt::U_BYTE; };

		template<>
		IndexInternalFmt GetIndexInternalFmt<uint16_t>() { return IndexInternalFmt::U_SHORT; }

		template<>
		IndexInternalFmt GetIndexInternalFmt<uint32_t>() { return IndexInternalFmt::U_INT; }
	}
}