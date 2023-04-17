#include "function/render/scene/Layer/RenderLayer.h"

namespace Pionner
{
    RenderLayer::RenderLayer(const std::shared_ptr<Rhi>& rhi)
        : m_rhi(rhi)
    {
    }

    RenderLayer::~RenderLayer()
    {
        m_rhi.reset();
    }

}