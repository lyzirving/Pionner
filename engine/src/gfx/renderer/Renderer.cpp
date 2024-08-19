#include "Renderer.h"

#include "DefferedRenderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Renderer"

namespace pio
{
	Ref<Renderer> Renderer::Create(const RenderConfig &config)
	{
		switch(config.Mode)
		{
			case RenderingMode_Deffered:
				return CreateRef<DefferedRenderer>();
			case RenderingMode_Forward:
			default:
				LOGE("err! Rendering mode[%u] has been implemented", config.Mode);
				std::abort();
				return Ref<Renderer>();
		}		
	}
}