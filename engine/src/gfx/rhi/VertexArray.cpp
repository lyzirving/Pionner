#include "VertexArray.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLVertexArray.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VertexBuffer"

namespace pio
{
	Ref<VertexArray> VertexArray::Create(Ref<RenderContext>& context)
	{
		switch (context->backendFlag())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLVertexArray>(context);
		default:
			LOGE("err! current backend[%u] has not been implemented", context->backendFlag());
			std::abort();
			return Ref<VertexArray>();
		}
	}
}