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
		virtual void draw(const std::shared_ptr<Camera> &camera) override;

		void addEntities(const std::vector<std::shared_ptr<RenderEntity>> &entities);

	private:
		static bool entitySorter(const std::shared_ptr<RenderEntity> &lhs, const std::shared_ptr<RenderEntity> &rhs);

	private:
		std::vector<std::shared_ptr<RenderEntity>> m_entities;
		bool                                       m_needSort;
	};
}

#endif