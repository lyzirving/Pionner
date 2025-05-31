#include "FrameBufferProxy.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "FrameBufferProxy"

namespace pio
{
	FrameBufferProxy::FrameBufferProxy(const Ref<RenderContext>& context, const std::string& name)
		: FrameBuffer(context, name)
	{
	}
}