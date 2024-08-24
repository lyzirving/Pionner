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
		switch(config.Strategy)
		{
			case RenderingStrategy::Deffered:
				return CreateRef<DefferedRenderer>();
			case RenderingStrategy::Forward:
			default:
				LOGE("err! Rendering mode[%u] has been implemented", config.Strategy);
				std::abort();
				return Ref<Renderer>();
		}		
	}
}