#include "VertexBuffer.h"

#include "gfx/renderer/RenderContext.h"

#include "gfx/rhi/opengl/GLVertexBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VertexBuffer"

namespace pio
{
	Ref<VertexBuffer> VertexBuffer::Create(Ref<RenderContext>& context, uint32_t size, BufferUsage usage)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLVertexBuffer>(context, size, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->renderBackend());
			std::abort();
			return Ref<VertexBuffer>();
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(Ref<RenderContext>& context, const void* data, uint32_t size, BufferUsage usage)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLVertexBuffer>(context, data, size, usage);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->renderBackend());
			std::abort();
			return Ref<VertexBuffer>();
		}
	}

	template<>
	VertexBufferLayout VertexBuffer::To<Vertex3d>()
	{
		VertexBufferElement pos{ "Pos", ShaderDataType::Float3, false };
		VertexBufferElement normal{ "Normal", ShaderDataType::Float3, false };
		VertexBufferElement texCoord{ "TexCoord", ShaderDataType::Float2, false };
		VertexBufferLayout layout{ pos, normal, texCoord };
		return layout;
	}
}