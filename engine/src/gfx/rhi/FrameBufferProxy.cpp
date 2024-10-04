#include "FrameBufferProxy.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBufferProxy"

namespace pio
{
	FrameBufferProxy::FrameBufferProxy(Ref<RenderContext>& context, const std::string& name)
		: FrameBuffer(context, name)
	{
	}

	FrameBufferProxy::FrameBufferProxy(Ref<RenderContext>& context, const std::string& name, uint32_t width, uint32_t height)
		: FrameBuffer(context, name)
	{		
	}
}