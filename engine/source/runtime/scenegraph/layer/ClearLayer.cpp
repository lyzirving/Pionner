#include "ClearLayer.h"

#include "render/rhi/RhiHeader.h"

namespace pio
{
	namespace sgf
	{
		ClearLayer::ClearLayer() : Layer(LAYER_TYPE_CLEAR)
		{
		}

		ClearLayer::~ClearLayer() = default;

		void ClearLayer::update(RenderInfo &info)
		{
		}

		/*void ClearLayer::draw(RenderParam &param)
		{
			auto rhi = param.rhi;

			const RenderViewport &port = param.renderViewport;
			rhi->setViewport(port.m_left, port.m_top, port.m_width, port.m_height);

			rhi->clearColor(m_color);
			rhi->clear(COLOR_BUF_BIT | DEPTH_BUF_BIT);
		}*/
	}
}