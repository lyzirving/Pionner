#include "RenderAPI.h"

#include "gfx/rhi/opengl/GLRenderAPI.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderAPI"

namespace pio
{
	Ref<RenderAPI> RenderAPI::Create(RenderBackendFlags flag)
	{
		switch(flag)
		{
			case RenderBackend_OpenGL:
				return CreateRef<GLRenderAPI>(flag);
			default:
				LOGE("Render backend[%u] has not been implemented", flag);
				std::abort();
				return Ref<RenderAPI>();
		}
	}
}