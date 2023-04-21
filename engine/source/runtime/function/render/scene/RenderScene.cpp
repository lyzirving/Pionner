#include "function/render/scene/RenderScene.h"
#include "function/render/scene/Camera.h"
#include "function/render/scene/Frustum.h"
#include "function/render/scene/layer/ClearLayer.h"
#include "function/render/scene/layer/ModelLayer.h"
#include "function/render/entity/RenderEntity.h"
#include "function/render/rhi/Rhi.h"
#include "function/render/RenderDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderScene"

namespace Pionner
{
	RenderScene::RenderScene() : m_rhi(nullptr), m_layers()
	{
	}

	RenderScene::~RenderScene()
	{
		shutdown();
	}

	void RenderScene::initialize(const std::shared_ptr<Rhi> &rhi)
	{
		m_rhi = rhi;

		for (uint8_t type = 0; type < LAYER_COUNT; type++)
		{
			std::shared_ptr<RenderLayer> layer = createLayer(SceneLayerType(type), m_rhi);
			if (layer) m_layers[type] = layer;
		}
	}

	void RenderScene::shutdown()
	{
		for (uint8_t i = 0; i < LAYER_COUNT; i++)
		{
			if (m_layers[i]) m_layers[i].reset();
		}
		m_rhi.reset();
	}

	void RenderScene::addEntities(const std::vector<std::shared_ptr<RenderEntity>> &entities)
	{
		if (m_layers[LAYER_MODEL])
		{
			ModelLayer *layer = (ModelLayer *)(m_layers[LAYER_MODEL].get());
			layer->addEntities(entities);
		}
	}

	void RenderScene::forwardRender(const RenderParam &param)
	{
		for (uint8_t type = 0; type < LAYER_COUNT; type++)
		{
			if (m_layers[type])
				m_layers[type]->draw(param);
		}
	}

	std::shared_ptr<RenderLayer> RenderScene::createLayer(SceneLayerType type, const std::shared_ptr<Rhi> &rhi)
	{
		std::shared_ptr<RenderLayer> layer{ nullptr };
		switch (type)
		{
			case Pionner::LAYER_CLEAR:
				layer = std::shared_ptr<RenderLayer>(new ClearLayer(rhi));
				break;
			case Pionner::LAYER_MODEL:
				layer = std::shared_ptr<RenderLayer>(new ModelLayer(rhi));
				break;
			case Pionner::LAYER_COUNT:
			default:
				LOG_ERR("invalid type[%u] for layer", type);
				break;
		}
		return layer;
	}

}