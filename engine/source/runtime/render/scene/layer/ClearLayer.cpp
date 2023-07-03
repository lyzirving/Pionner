#include "render/scene/layer/ClearLayer.h"
#include "render/scene/Camera.h"
#include "render/scene/Frustum.h"

#include "render/rhi/Rhi.h"

#include "render/RenderDef.h"

namespace pio
{
	ClearLayer::ClearLayer() : RenderLayer(), m_color(0.24f, 0.24f, 0.24f, 1.f)
	{
	}

	ClearLayer::~ClearLayer() = default;

	void ClearLayer::draw(RenderParam &param)
	{
		auto rhi = param.rhi;

		const RenderViewport &port = param.renderViewport;
		rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);

		rhi->clearColor(m_color);
		rhi->clear(COLOR_BUF_BIT | DEPTH_BUF_BIT);
	}
}