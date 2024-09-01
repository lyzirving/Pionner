#include "FrameBuffer.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/rhi/opengl/GLFrameBuffer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBuffer"

namespace pio
{
	Ref<FrameBuffer> FrameBuffer::Create(Ref<RenderContext>& context, const FrameBufferSpecific& spec)
	{
		switch (context->renderBackend())
		{
		case RenderBackend_OpenGL:
			return CreateRef<GLFrameBuffer>(context, spec);
		default:
			LOGE("err! backend[%u] has not been implemented", context->renderBackend());
			std::abort();
			return Ref<FrameBuffer>();
		}
	}
}