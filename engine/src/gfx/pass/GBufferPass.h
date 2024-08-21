#ifndef __PIONNER_GFX_PASS_GBUFFER_PASS_H__
#define __PIONNER_GFX_PASS_GBUFFER_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class GBufferPass : public RenderPass
	{
	public:
		GBufferPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~GBufferPass() = default;
	};
}

#endif