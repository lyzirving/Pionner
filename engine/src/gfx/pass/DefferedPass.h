#ifndef __PIONNER_GFX_PASS_DEFFERED_PASS_H__
#define __PIONNER_GFX_PASS_DEFFERED_PASS_H__

#include "RenderPass.h"

namespace pio
{
	class DefferedPass : public RenderPass
	{
	public:
		DefferedPass(const std::string &name, RenderPassEvent event) : RenderPass(name, event) {}
		~DefferedPass() = default;
	};
}

#endif