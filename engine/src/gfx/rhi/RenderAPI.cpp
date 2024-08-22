#include "RenderAPI.h"

#include "gfx/rhi/opengl/GLRenderAPI.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderAPI"

namespace pio
{
	Ref<RenderAPI> RenderAPI::Create(RenderBackendFlags type)
	{
		switch(type)
		{
			case RenderBackend_OpenGL:
				return CreateRef<GLRenderAPI>(type);
			default:
				LOGE("RenderAPI type[%u] has not been implemented", type);
				std::abort();
				return Ref<RenderAPI>();
		}
	}
}