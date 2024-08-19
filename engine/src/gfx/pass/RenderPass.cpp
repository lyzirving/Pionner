#include "RenderPass.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderPass"

namespace pio
{
	bool RenderPass::PassSorter(Ref<RenderPass> &lhs, Ref<RenderPass> &rhs) 
	{
		return lhs->event() < rhs->event();
	}

	BlockRange RenderBlock::GetBlockRange(RenderBlockFlags flag)
	{
		switch (flag)
		{
		case MainBeforeRendering:
			return BlockRange(RenderPassEvent(0), BeforeRenderingShadows);
		case MainRenderingOpaque:
			return BlockRange(BeforeRenderingShadows, BeforeRenderingTransparents);
		case MainRenderingTransparents:
			return BlockRange(BeforeRenderingTransparents, BeforeRenderingPostProcessing);
		case MainAfterRendering:
			return BlockRange(BeforeRenderingPostProcessing, AfterRendering);
		default:
			LOGE("Err! Undefined RenderBlockFlag[%u]", flag);
			std::abort();
			return BlockRange();
		}
	}
}