#ifndef __RENDER_MODEL_LAYER_H__
#define __RENDER_MODEL_LAYER_H__

#include <vector>

#include "function/render/scene/layer/RenderLayer.h"

namespace Pionner
{
	class RenderEntity;

	class ModelLayer : public RenderLayer
	{
	public:
		ModelLayer(const std::shared_ptr<Rhi> &rhi);
		virtual ~ModelLayer();

		virtual void clear() override;
		virtual void draw() override;

		void addEntities(const std::vector<std::shared_ptr<RenderEntity>> &entities);

	private:
		std::vector<std::shared_ptr<RenderEntity>> m_entities;
		bool                                       m_needSort;
	};
}

#endif