#include "function/render/scene/RenderScene.h"
#include "function/render/scene/layer/RenderLayer.h"
#include "function/render/interface/Rhi.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderScene"

namespace Pionner
{
	RenderScene::RenderScene(const std::shared_ptr<Rhi>& rhi) 
		: m_rhi(rhi), m_layers()
	{
	}

	RenderScene::~RenderScene()
	{
		for (uint8_t i = 0; i < LAYER_COUNT; i++)
		{
			if (m_layers[i]) m_layers[i].reset();
		}
		m_rhi.reset();
	}

	void RenderScene::initialize()
	{
		/*for (uint8_t type = 0; type < LAYER_COUNT; type++)
		{
			std::shared_ptr<RenderLayer> layer = createLayer(SceneLayerType(type));
			if (layer) m_layers[type] = layer;
		}*/
	}

	void RenderScene::shutdown()
	{
	}

	std::shared_ptr<RenderLayer> RenderScene::createLayer(SceneLayerType type)
	{
		switch (type)
		{
		case Pionner::LAYER_CLEAR:
			break;
		case Pionner::LAYER_MODEL:
			break;
		case Pionner::LAYER_COUNT:
		default:
		{
			LOG_ERR("invalid type[%u] for layer", type);
			return std::shared_ptr<RenderLayer>();
		}
		}
	}

}