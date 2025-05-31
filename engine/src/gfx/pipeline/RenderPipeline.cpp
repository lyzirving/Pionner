#include "RenderPipeline.h"

#include "GlobalSettings.h"

#include "gfx/renderer/RenderContext.h"
#include "gfx/renderer/Renderer.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPipeline"

namespace pio
{
	RenderPipeline::RenderPipeline(const Ref<RenderContext>& context) : m_Context(context)
	{
	}

	void RenderPipeline::Render(const Ref<RenderContext>& context, const std::vector<Ref<CameraNode>>& camNodes)
	{
		context->OnBeginFrameRendering();
		for(int32_t i = 0; i < camNodes.size(); i++)
		{						
			context->GetRenderer()->OnExecute(context, camNodes[i]);			
		}
		context->OnEndFrameRendering();
	}
}