#include "MainLightShadowCasterPass.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "MainLightShadowCasterPass"

namespace pio
{
	template<>
	bool RenderPass::is<MainLightShadowCasterPass>() const { return type() == RenderPassType::MainLightShadowCaster; }
}