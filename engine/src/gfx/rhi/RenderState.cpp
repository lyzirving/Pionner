#include "RenderState.h"

#include "gfx/rhi/opengl/GLRenderState.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderState"

namespace pio
{
	Ref<RenderState> RenderState::Create(RenderBackendFlags flag)
	{
		switch (flag)
		{
			case RenderBackend_OpenGL:
				return CreateRef<GLRenderState>(flag);
			default:
				LOGE("Render backend[%u] has not been implemented", flag);
				std::abort();
				return Ref<RenderState>();
		}
	}
}