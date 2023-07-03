#include "render/scene/RenderScene.h"
#include "render/scene/Camera.h"
#include "render/scene/Frustum.h"
#include "render/scene/layer/ClearLayer.h"
#include "render/scene/layer/ModelLayer.h"
#include "render/rhi/Rhi.h"
#include "render/RenderDef.h"

#include "core/log/LogSystem.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RenderScene"

namespace pio
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
			std::shared_ptr<RenderLayer> layer = createLayer(SceneLayerType(type));
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

	void RenderScene::forwardRender(RenderParam &param)
	{
		for (uint8_t type = 0; type < LAYER_COUNT; type++)
		{
			if (m_layers[type])
				m_layers[type]->draw(param);
		}
	}

	std::shared_ptr<RenderLayer> RenderScene::createLayer(SceneLayerType type)
	{
		std::shared_ptr<RenderLayer> layer{ nullptr };
		switch (type)
		{
			case pio::LAYER_CLEAR:
				layer = std::shared_ptr<RenderLayer>(new ClearLayer);
				break;
			case pio::LAYER_MODEL:
				layer = std::shared_ptr<RenderLayer>(new ModelLayer);
				break;
			case pio::LAYER_COUNT:
			default:
				LOG_ERR("invalid type[%u] for layer", type);
				break;
		}
		return layer;
	}

}