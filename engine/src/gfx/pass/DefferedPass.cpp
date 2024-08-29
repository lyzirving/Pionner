#include "DefferedPass.h"

#include "gfx/renderer/RenderContext.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "DefferedPass"

namespace pio
{
    void DefferedPass::execute(Ref<RenderContext> &context)
    {
        auto &renderingData = context->renderingData();
        if(renderingData.OpaqueMeshCmd.empty())
            return;
    }
}