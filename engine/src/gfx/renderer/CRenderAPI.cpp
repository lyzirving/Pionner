#include "CRenderAPI.h"

#include "platform/opengl/GLRenderAPI2.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "CRenderAPI"

namespace pio
{
	Ref<CRenderAPI> CRenderAPI::Create(BackendFlags type)
	{
		switch(type)
		{
			case Backend_OpenGL:
				return CreateRef<GLRenderAPI2>(type);
			default:
				LOGE("RenderAPI type[%u] has not been implemented", type);
				std::abort();
				return Ref<CRenderAPI>();
		}
	}
}