#include "Scene.h"

#include "layer/Layer.h"

namespace pio
{
	namespace sgf
	{
		Scene::Scene()
		{
			createLayers();
		}

		Scene::~Scene()
		{
			for (auto &layer : m_layers)
			{
				layer.reset();
			}
		}

		void Scene::update(RenderInfo &info)
		{
			for (auto &layer : m_layers)
			{
				if (layer)
					layer->update(info);
			}
		}

		void Scene::createLayers()
		{
		}
	}
}