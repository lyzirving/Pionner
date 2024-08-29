#include "FrameBuffer.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBuffer"

namespace pio
{
	Ref<FrameBuffer> FrameBuffer::Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec)
	{
		switch (context->backendFlag())
		{
		case RenderBackend_OpenGL:
			return Ref<FrameBuffer>();
		default:
			LOGE("err! backend[%u] has not been implemented", context->backendFlag());
			std::abort();
			return Ref<FrameBuffer>();
		}
	}
}