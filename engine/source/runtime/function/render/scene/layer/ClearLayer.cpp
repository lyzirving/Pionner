#include "function/render/scene/layer/ClearLayer.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"

#include "function/render/rhi/Rhi.h"

#include "function/render/RenderDef.h"

namespace Pionner
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