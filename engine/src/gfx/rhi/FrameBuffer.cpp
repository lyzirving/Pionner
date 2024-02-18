#include "FrameBuffer.h"

#include "gfx/rhi/RenderAPI.h"
#include "platform/opengl/GLFrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBuffer"

namespace pio
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification &spec)
	{
		switch (RenderAPI::CurrentType())
		{
			case RenderAPI::Type::OpenGL:
				return CreateRef<GLFrameBuffer>(spec);
			default:
				return Ref<FrameBuffer>();
		}
	}
}