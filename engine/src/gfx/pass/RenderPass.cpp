#include "RenderPass.h"

namespace pio
{
	bool RenderPass::PassSorter(Ref<RenderPass> &lhs, Ref<RenderPass> &rhs) 
	{
		return lhs->event() < rhs->event();
	}
}