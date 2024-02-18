#include "VertexArray.h"

#include "gfx/rhi/RenderAPI.h"

#include "platform/opengl/GLVertexArray.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "VertexBuffer"

namespace pio
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<VertexArray, GLVertexArray>();
			default:
				return Ref<VertexArray>();
		}
	}
}