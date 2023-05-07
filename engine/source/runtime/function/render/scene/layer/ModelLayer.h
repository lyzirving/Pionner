#ifndef __RENDER_MODEL_LAYER_H__
#define __RENDER_MODEL_LAYER_H__

#include <vector>

#include "function/render/scene/layer/RenderLayer.h"

namespace Pionner
{
	class ModelLayer : public RenderLayer
	{
	public:
		ModelLayer(const std::shared_ptr<Rhi> &rhi);
		virtual ~ModelLayer();

		virtual void draw(RenderParam &param) override;
	};
}

#endif