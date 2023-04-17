#ifndef __RENDER_LAYER_H__
#define __RENDER_LAYER_H__

#include <memory>

namespace Pionner
{
	class Rhi;

	class RenderLayer
	{
	public:
		RenderLayer(const std::shared_ptr<Rhi> &rhi);
		virtual ~RenderLayer();

		virtual void clear();
		virtual void draw() = 0;

	protected:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif