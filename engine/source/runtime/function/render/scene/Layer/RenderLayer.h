#ifndef __RENDER_LAYER_H__
#define __RENDER_LAYER_H__

#include <memory>

namespace Pionner
{
	class Rhi;
	class Camera;
	class Frustum;

	class RenderLayer
	{
	public:
		RenderLayer(const std::shared_ptr<Rhi> &rhi);
		virtual ~RenderLayer();

		virtual void clear();
		virtual void draw(const std::shared_ptr<Camera> &camera, const std::shared_ptr<Frustum> &frustum) = 0;

	protected:
		std::shared_ptr<Rhi> m_rhi;
	};
}

#endif